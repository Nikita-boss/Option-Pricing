// Used when incorrect engine type was passed to an option object

#ifndef INCORRECTENGINEEXCEPTION_HPP
#define INCORRECTENGINEEXCEPTION_HPP

#include "EngineException.hpp"
#include <sstream>
#include <string>

namespace PricingLibrary {
	 
	class IncorrectEngineException : public EngineException
	{
	private:
		std::string _message;

	public:
		IncorrectEngineException(const std::string& message) : _message(message) {} // default constructor
		~IncorrectEngineException() {}   // default destructor

		std::string what() const // print exception message
		{
			std::stringstream ss;
			ss << "Incorrect Pricing Engine.\n" << _message;

			return ss.str();
		}
	};
}

#endif