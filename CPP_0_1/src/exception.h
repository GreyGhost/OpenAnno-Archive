#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED
#include <exception>
#include <string>
#include <sstream>

class FileLoadException : public std::exception
{
	public:
		explicit FileLoadException(const std::string& filename, const std::string& error)
		:
			mFilename(filename),
			mError(error)
		{
		}

		virtual const char* what() const throw()
		{
			std::string error;
			error = "Error loading file »" + mFilename + "«: " + mError;
			return error.c_str();
		}

		virtual ~FileLoadException() throw()
		{
		}

	protected:
		std::string mFilename;
		std::string mError;
}; 

class XMLException : public std::exception
{
	public:
		explicit XMLException(const std::string& file, int line, const std::string& error)
		:
			mFile(file),
			mLine(line),
			mError(error)
		{
		}
		
		virtual const char* what() const throw()
		{
			std::string errmsg = "XML Error (in file " + mFile;
			if (mLine > 0)
			{
				std::stringstream itos;
				itos << mLine;
				errmsg += ":" + itos.str();
			}
			errmsg += "): " + mError;
			return errmsg.c_str();
		}
		
		virtual ~XMLException() throw()
		{
		}

	protected:
		std::string mFile;
		int mLine;
		std::string mError;
};

#endif
