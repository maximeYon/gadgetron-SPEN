classdef SPEN_bufferRecon < handle & BaseBufferGadget
    
    properties
        %% declare the variable name used in the g. structure
        image_num;
        series_num;
        
        compteur;
        %         full_matrice;
        parameters;
        SPEN_parameters;
        ParamSR;
        FinalAFull;
        FinalA;
        
    end
    
    methods
        
        function g = config(g)
            g.image_num      = 0;  % todo this needs to be static or global...
            g.series_num     = 0;  % todo this needs to be static or global...
            
            %% Retrieve requiered parameters and store them in g. structure
            g.parameters.matrixSize(1,1) = g.xml.encoding.encodedSpace.matrixSize.x;
            g.parameters.matrixSize(1,2) = g.xml.encoding.encodedSpace.matrixSize.y;
            g.parameters.matrixSize(1,3) = g.xml.encoding.encodedSpace.matrixSize.z;
            g.parameters.FOV(1,1) = g.xml.encoding.encodedSpace.fieldOfView_mm.x;
            g.parameters.FOV(1,2) = g.xml.encoding.encodedSpace.fieldOfView_mm.y;
            g.parameters.FOV(1,3) = g.xml.encoding.encodedSpace.fieldOfView_mm.z;
%             g.parameters.matrixSizeReco2 = g.xml.encoding.reconSpace.matrixSize.y;
            
            g.parameters.Enc1 = g.xml.encoding.encodingLimits.kspace_encoding_step_1.maximum+1;
            g.parameters.Enc2 = g.xml.encoding.encodingLimits.kspace_encoding_step_2.maximum+1;
            g.parameters.slice = g.xml.encoding.encodingLimits.slice.maximum+1;
            g.parameters.set = g.xml.encoding.encodingLimits.set.maximum+1;
            g.parameters.segment = round(g.parameters.matrixSize(1,2)/g.parameters.Enc1);
            g.parameters.matrixSize(1,2) = g.parameters.Enc1*g.parameters.segment;
            g.parameters.repetition = g.parameters.set/g.parameters.segment;
            g.parameters.echo_spacing = g.xml.sequenceParameters.echo_spacing*1000;
            
            % get user parameters
            for ind = 1:size(g.xml.userParameters.userParameterLong,2)
                g.parameters.(g.xml.userParameters.userParameterLong(1,ind).name) = g.xml.userParameters.userParameterLong(1,ind).value;
            end
            clearvars ind
            
 
           g_saved = struct(g);
           save('/home/mygadg/Documents/MATLAB/SPEN_data_test_param.mat','g_saved');

            
            %% Get parameters to compute PEshift
            % PE shift is independant to the slice position
            idx_data=find(head.idx.kspace_encode_step_1~=0);
            idx_data = idx_data(1,1);
            
            % Get the position in read phase slice
            PosXYZ = head.position(idx_data,:);
            
            % Get the orientation
            RotMatread = head.read_dir(:,idx_data);
            RotMatphase = head.phase_dir(:,idx_data);
            RotMatslice = head.slice_dir(:,idx_data);
            RotMat = [RotMatphase,RotMatread,RotMatslice]; % Is is order alway good ?
            
            RotatedLocs=RotMat.'*PosXYZ;
            PEShift=RotatedLocs(1,1);
            PEShiftSign=-1;
            g.parameters.PEShift=PEShiftSign*PEShift;
            
            %% Create compteur and full matrice to store the data
            g.compteur=0;
            
            %% add to path requiered functions
            addpath('/usr/local/share/ismrmrd/matlab')
            addpath(genpath('/home/imagerie/Documents/MATLAB/save_SPEN_files/SPEN_Gadgetron_Reco/SPEN_Siemens_Recon/'))
        end
        
        function g = process(g, recon_data)
            fprintf('Processing')
            %% get the data and header
            % header is need to save the data
            data = recon_data(1).data; %Assume only one encoding space
            head = data.headers; %Just get header from first trajectory
            CmplxData = data.data;
            clearvars data;
            
            tic
            %% Retrieve the parameters and compute the super resolution matrix
            %% used for deconvolution of all sets
            if g.compteur==0
                %% Get specific SPEN parameters
                g.SPEN_parameters = struct;
                FOV = g.parameters.FOV;g.SPEN_parameters.LPE = FOV(1,2)/10;
                g.SPEN_parameters.rvalue = g.parameters.rvalue;
                g.SPEN_parameters.nReps = 1; %% Need to be changed !!!!!!!!!!!!!!!!!
                matrixSize = g.parameters.matrixSize;  g.SPEN_parameters.Npe = matrixSize(1,2);
                g.SPEN_parameters.nShots = g.parameters.segment;
                g.SPEN_parameters.EchoSpacing = g.parameters.echo_spacing;
                g.SPEN_parameters.PEShift = g.parameters.PEShift;
                g.SPEN_parameters.nWeight = g.parameters.repetition;
                g.SPEN_parameters.nSlices = g.parameters.slice;
                
                %% Define user parameter
                g.SPEN_parameters.TVW_L2.Final=0.15;  % Weight for L2 regularization for final reconstruction
                g.SPEN_parameters.TVW_L2.EO=0.6;  % Weight for L2 regularization for reconstruction of even and odd images used to determine EO phase correction
                g.SPEN_parameters.TVW_L2.ImgPerShot=0.6;  % Weight for L2 regularization for reconstruction of per-shot images used to perform motion correction
                g.SPEN_parameters.TresholdCC=0.15; % Threshold to determine to how many coils to compress the data
                g.SPEN_parameters.WeightingFactor=100; % Weighting factor used for super-resolution (SR) matrix calculation
                g.SPEN_parameters.DoMotionCorrection=false; % Set true for in-vivo data, false for phantom data, relevant only for data acquired with #_of_segments > 1
                
                %% Assign the parameters used for the Super-Resolution matrix
                g.ParamSR.LPE=g.SPEN_parameters.LPE; % FOV along PE direction [cm]
                g.ParamSR.rvalue=-g.SPEN_parameters.rvalue; % CHIRP rvalue
                g.ParamSR.EchoSpacing=g.SPEN_parameters.EchoSpacing; % Echo spacing [us]
                if g.SPEN_parameters.nShots>1
                    g.ParamSR.PEShift=g.SPEN_parameters.PEShift; % Shift for data along PE direction to remove shift added by EPI read-out
                else
                    g.ParamSR.PEShift=g.SPEN_parameters.PEShift; % Shift for data along PE direction to remove shift added by EPI read-out
                end
                
                %% SuperResolution (SR) matrix calculation
                % SR matrix depends on the sequence in use. This
                % calculation is for 180deg encoding CHIRP and another 180deg
                % slice-non-selective inversion pulse. If you use different sequence, you might need
                % to change some parameters in the function (or write your own calculation
                % of SR matrix).
                g.SPEN_parameters.ChirpAmpExtraFactor=1;
                
                % this is for the final reconstruction (size equal to final data size)
                [g.FinalAFull]=SuperResolution_VariableSize_Sim_V3(g.SPEN_parameters.Npe,g.ParamSR,1,g.SPEN_parameters.WeightingFactor,g.SPEN_parameters.nShots,g.SPEN_parameters.ChirpAmpExtraFactor);
                
                %this is for phase per shot used for motion correction
                g.FinalA=PartitionDimInterleaved(g.FinalAFull,1,g.SPEN_parameters.nShots);
                
            end
            
            %% Perform the reconstruction for each set of image
            g.compteur = g.compteur+1;
            disp([' number of set = ' num2str(g.compteur)])
            
            %% Create CmplxData size(120 120 12 3 1 1 13) for SPENreco
            % Crop the data in dim 2 : only the center contain the data due
            % to the segmented acquistion and wrong tagging
            CmplxData = CmplxData(:,size(CmplxData,2)/2-g.parameters.Enc1/2+1:size(CmplxData,2)/2+g.parameters.Enc1/2,:,:,:,:,:,:);
            % Rearrange the segment
            CmplxData = permute(CmplxData, [1 2 5 3 4 6 7 8]);
            shot_coord = 1:g.SPEN_parameters.nShots:g.SPEN_parameters.nShots*size(CmplxData,2);
            CmplxDataSeg = zeros(size(CmplxData,1),size(CmplxData,2)*size(CmplxData,3),size(CmplxData,4),size(CmplxData,5),size(CmplxData,6),size(CmplxData,7));
            for shot = 1:g.SPEN_parameters.nShots
                CmplxDataSeg(:,shot_coord+shot-1,:,:,:,:) = CmplxData(:,:,shot,:,:,:,:);
            end
            CmplxDataSeg = reshape(CmplxDataSeg,size(CmplxData,1),size(CmplxDataSeg,2),size(CmplxData,5),size(CmplxData,7),1,1,size(CmplxData,8));
            CmplxData = CmplxDataSeg;
            clearvars CmplxDataSeg;
            
            %% permute dim 1 and  2
            CmplxData = permute(CmplxData,[2 1 3 4 5 6 7]);
            
            
            %            %% Perform zeros filling to match the number of Reconstruction points in dimension SPEN
            %            number_ZF_points =g.parameters.matrixSizeReco2 -size(CmplxData,1);
            %             if number_ZF_points ~=0
            %                 if mod(number_ZF_points,2)==0; % paire
            %                     zf_mat = zeros(number_ZF_points/2,size(CmplxData,2),size(CmplxData,3));
            %                     CmplxData = cat(1,zf_mat,CmplxData,zf_mat);
            %                 else % impaire
            %                     zf_mat1 = zeros(floor(number_ZF_points/2),size(CmplxData,2),size(CmplxData,3));
            %                     zf_mat2 = zeros(ceil(number_ZF_points/2),size(CmplxData,2),size(CmplxData,3));
            %                     CmplxData = cat(1,zf_mat1,CmplxData,zf_mat2);
            %                 end
            %             end
            
            %% SPEN reconstruction
            % PE Shift: Shift data along PE direction to remove phase added by EPI read-out
            CmplxData = PEShiftingDataset(CmplxData,-g.SPEN_parameters.PEShift,g.SPEN_parameters.LPE,g.SPEN_parameters.nShots);
            
            %% Coil Compression
            % Determine number of virtual channels to which compress data, but still
            % not  to loose too much information (TresholdCC). If this condition,
            % is not reached, we will compress to three coils.
            if g.compteur==1
                [~, g.SPEN_parameters.gccmtx_aligned_finalx, Energyx]=CoilCompression(permute(CmplxData,[2 1 3 4 5 6 7]),1);
                g.SPEN_parameters.nccx=find(Energyx(:,1)<g.SPEN_parameters.TresholdCC,1);
                if isempty(g.SPEN_parameters.nccx) && size(CmplxData,3)>2
                    g.SPEN_parameters.nccx=3;
                elseif isempty(g.SPEN_parameters.nccx)
                    g.SPEN_parameters.nccx=1;
                end
            end
            
            %% ONE SHOT Reconstruction
            % Description of the workflow given inside the script. You might want to
            % change L2 weight.
            if g.SPEN_parameters.nShots==1
                [SPEN_Image]=function_OneShotReconstruction(CmplxData,g.SPEN_parameters.gccmtx_aligned_finalx,g.SPEN_parameters.nccx,g.FinalAFull,g.SPEN_parameters);
                % OneShotReconstruction
            else
                %% Determine SENS map, as well as EO map
                % Determine EO maps for each slice from B0 data
                % The even-odd maps are assumed not to change for other b-weigted images and repetitions,
                % hence, they will be later applied for them also.
                % More explanations inside the function
                if g.compteur==1
                    RepsToDoEOAndB0=g.SPEN_parameters.nReps;
                    [g.SPEN_parameters.EvenOddMapStoredBySlices, g.SPEN_parameters.FinalB0Recon]=SensAndEOMapFromB0(CmplxData, g.FinalAFull, g.SPEN_parameters.gccmtx_aligned_finalx, g.SPEN_parameters.nccx, g.SPEN_parameters.nShots,RepsToDoEOAndB0,g.SPEN_parameters.TVW_L2);
                end
                %% Final reconstruction
                [SPEN_Image]=FinalReconstruction(CmplxData, g.SPEN_parameters.gccmtx_aligned_finalx, g.SPEN_parameters.nccx, g.SPEN_parameters.EvenOddMapStoredBySlices, g.FinalA, g.FinalAFull, g.SPEN_parameters.FinalB0Recon, g.SPEN_parameters.nShots, g.SPEN_parameters.TVW_L2, g.SPEN_parameters.DoMotionCorrection);
            end
            SPEN_Image=permute(SPEN_Image, [2,1,3]);
            SPEN_Image = flip(flip(SPEN_Image,2),1);
            
            time_reco = toc;
            disp(['Duration of SPEN reco = ' num2str(time_reco) ' s'])
            
            %             %% store the data and header at last repetition
            %             g.full_matrice(:,:,:,g.compteur) = squeeze(SPEN_Image);
            %                         if g.compteur==g.parameters.repetition
            %                             g_saved = struct(g);
            %                             save('/home/imagerie/Documents/MATLAB/SPEN_data_test.mat','g_saved','head','SPEN_Image');
            %                         end
            %
            % Display
                                    figure(10)
                                    for dim3 = 1: size(SPEN_Image,3)
                                        subplot(1,size(SPEN_Image,3),dim3)
                                        imagesc(abs(squeeze(SPEN_Image(:,:,dim3,1,1))))
                                        colormap gray
                                    end
                                    pause(10)
            
            
            % At the end of the acquisition, reconstruct the slice
            for s=1:size(SPEN_Image,3)
                
                img_head = ismrmrd.ImageHeader;
                
                % find the good header parameters for each slice
                idx_data=find(head.idx.kspace_encode_step_1==(head.idx.slice==s-1));
                idx_data = idx_data(1,1);
                
                % set the matrix size
                % set one element at a time to not break the type (uint16) of matrix_size
                img_head.matrix_size(1) = size(SPEN_Image,1);
                img_head.matrix_size(2) = size(SPEN_Image,2);
                img_head.matrix_size(3) = 1;
                
                img_head.field_of_view(1) = g.parameters.FOV(1,1);
                img_head.field_of_view(2) = g.parameters.FOV(1,2);
                img_head.field_of_view(3) = g.parameters.FOV(1,3);
                
                
                img_head.position = head.position(:,idx_data);  %acq(1)
                img_head.read_dir = head.read_dir(:,idx_data);
                img_head.phase_dir = head.phase_dir(:,idx_data);
                img_head.slice_dir = head.slice_dir(:,idx_data);
                img_head.patient_table_position = head.patient_table_position(:,idx_data);
                img_head.acquisition_time_stamp = head.acquisition_time_stamp(idx_data);
                img_head.image_index = g.compteur*1000; %g.image_num;
                img_head.image_series_index = g.series_num;
                img_head.channels = 1;
                img_head.data_type= 7;
                
                %center = floor(size(img_data,3)/2)+1;
                %imagesc(abs(img_data(:,:,center,1,1,1,1))); axis image; axis square;
                %pause(2)
                %close()
                
                %disp(size(img_data));
                
                g.putImageQ(img_head, squeeze(SPEN_Image(:,:,s)));
                %fprintf('Put on Queue %d, type = %d\n',length(g.Q),g.Q{1}.type);
                
            end
        end
        
    end
end