#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <time.h>
#include <iostream>

namespace Gadgetron
{
  GadgetronLogger* GadgetronLogger::instance()
  {
    if (!instance_) instance_ = new GadgetronLogger();
    return instance_;
  }
  
  GadgetronLogger* GadgetronLogger::instance_ = NULL;
  
  GadgetronLogger::GadgetronLogger()
    : level_mask_(0)
    , print_mask_(0)
  {
    char* log_mask = getenv(GADGETRON_LOG_MASK_ENVIRONMENT);
    if ( log_mask != NULL) {
      
      std::string log_mask_str(log_mask);

      //Which log levels are enabled
      if (log_mask_str.find("ALL") != std::string::npos) {
	uint64_t null_mask;
	level_mask_ |= ~null_mask;
	print_mask_ |= ~null_mask;
	return;
      }
      
      if (log_mask_str.find("LEVEL_DEBUG") != std::string::npos) 
	enableLogLevel(GADGETRON_LOG_LEVEL_DEBUG);

      if (log_mask_str.find("LEVEL_INFO") != std::string::npos) 
	enableLogLevel(GADGETRON_LOG_LEVEL_INFO);

      if (log_mask_str.find("LEVEL_WARNING") != std::string::npos) 
	enableLogLevel(GADGETRON_LOG_LEVEL_WARNING);

      if (log_mask_str.find("LEVEL_ERROR") != std::string::npos) 
	enableLogLevel(GADGETRON_LOG_LEVEL_ERROR);

      if (log_mask_str.find("PRINT_FILELOC") != std::string::npos) 
	enableOutput(GADGETRON_LOG_PRINT_FILELOC);

      if (log_mask_str.find("PRINT_LEVEL") != std::string::npos) 
	enableOutput(GADGETRON_LOG_PRINT_LEVEL);
      
      if (log_mask_str.find("PRINT_DATETIME") != std::string::npos) 
	enableOutput(GADGETRON_LOG_PRINT_DATETIME);
    } else {
      enableLogLevel(GADGETRON_LOG_LEVEL_DEBUG);
      enableLogLevel(GADGETRON_LOG_LEVEL_INFO);
      enableLogLevel(GADGETRON_LOG_LEVEL_WARNING);
      enableLogLevel(GADGETRON_LOG_LEVEL_ERROR);
      enableOutput(GADGETRON_LOG_PRINT_FILELOC);
      enableOutput(GADGETRON_LOG_PRINT_LEVEL);
      enableOutput(GADGETRON_LOG_PRINT_DATETIME);
    }
  }


  void GadgetronLogger::log(GadgetronLogLevel LEVEL, const char* filename, int lineno, const char* cformatting, ...)
  {
    //Check if we should log this message
    if (!isLevelEnabled(LEVEL)) return;

    const char* fmt = cformatting;
    std::string fmt_str;
    bool append_cformatting_needed = false; //Will be set to true if we add any additional labels

    if (isOutputEnabled(GADGETRON_LOG_PRINT_DATETIME)) {
      time_t rawtime;
      struct tm * timeinfo;

      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      
      //Time the format YYYY-MM-DD HH:MM:SS
      char timestr[22];sprintf(timestr, "%d-%02d-%02d %02d:%02d:%02d ",
			       timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday,
			       timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

      fmt_str += std::string(timestr);
      append_cformatting_needed = true;
    }

    if (isOutputEnabled(GADGETRON_LOG_PRINT_LEVEL)) {
      switch (LEVEL) {
      case GADGETRON_LOG_LEVEL_DEBUG:
	fmt_str += "DEBUG ";
	break;
      case GADGETRON_LOG_LEVEL_INFO:
	fmt_str += "INFO ";
	break;
      case GADGETRON_LOG_LEVEL_WARNING:
	fmt_str += "WARNING ";
	break;
      case GADGETRON_LOG_LEVEL_ERROR:
	fmt_str += "ERROR ";
	break;
      default:
	;
      }
      append_cformatting_needed = true;
    }

    if (isOutputEnabled(GADGETRON_LOG_PRINT_FILELOC)) {
      char linenostr[8];sprintf(linenostr, "%d", lineno);
      fmt_str += std::string("[") + std::string(filename);
      fmt_str += std::string(":") + std::string(linenostr);
      fmt_str += std::string("] ");
      append_cformatting_needed = true;
    }

    if (append_cformatting_needed) {
      fmt_str += std::string(cformatting); 
      fmt = fmt_str.c_str();      
    }

    va_list args;
    va_start (args, cformatting);
    vprintf(fmt, args);
    va_end (args);
    fflush(stdout);
  }

  void GadgetronLogger::enableLogLevel(GadgetronLogLevel LEVEL)
  {
    if (LEVEL < GADGETRON_LOG_LEVEL_MAX) {
      level_mask_ |= (1<<LEVEL);
    }
  }

  void GadgetronLogger::disableLogLevel(GadgetronLogLevel LEVEL)
  {
    if (LEVEL < GADGETRON_LOG_LEVEL_MAX) {
      level_mask_ &= ~(1<<LEVEL);
    }
  }
  
  bool GadgetronLogger::isLevelEnabled(GadgetronLogLevel LEVEL)
  {
    if (LEVEL >= GADGETRON_LOG_LEVEL_MAX) return false;

    return ((1<<LEVEL) & level_mask_)>0;
  }
  
  void GadgetronLogger::enableOutput(GadgetronLogOutput OUTPUT) 
  {
    if (OUTPUT < GADGETRON_LOG_PRINT_MAX) {
      print_mask_ |= (1<<OUTPUT);
    }    
  }

  void GadgetronLogger::disableOutput(GadgetronLogOutput OUTPUT) 
  {
    if (OUTPUT < GADGETRON_LOG_PRINT_MAX) {
      print_mask_ &= ~(1<<OUTPUT);
    }    
  }
 
  bool GadgetronLogger::isOutputEnabled(GadgetronLogOutput OUTPUT)
  {
    if (OUTPUT >= GADGETRON_LOG_PRINT_MAX) return false;
    return ((1<<OUTPUT) & print_mask_)>0;
  }
}
