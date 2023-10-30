// Define general Pricing Engine Exception class

#ifndef ENGINEEXCEPTION_HPP
#define ENGINEEXCEPTION_HPP

#include <string>

namespace PricingLibrary {
	 
	class EngineException
	{
	public:
		EngineException() {} // default constructor
		virtual ~EngineException() {}    // default destructor

		virtual std::string what() const =0;  // print exception message
	};
}

#endif