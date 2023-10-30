// Implementation of header file Payoff.hpp

#include "Payoff.hpp"

namespace PricingLibrary {

    /// @brief Default constructor
    /// @param maturity
    /// @param strike 
    /// @param type 
    /// @param exercise 
    Payoff::Payoff(double maturity, double strike, Type type, Exercise exercise)
    : _maturity{maturity}, _strike{strike}, _type{type}, _exercise{exercise} {}

    /// @brief Copy constructor
    /// @param source Payoff object
    Payoff::Payoff(const Payoff& source)
    : _maturity{source._maturity}, _strike{source._strike}, _type{source._type}, _exercise{source._exercise}
    {}

    /// @brief Copy assignemnt
    /// @param source Payoff object
    /// @return Payoff object
    Payoff& Payoff::operator= (const Payoff& source)
    {
        // check for self-assignment
        if (this == &source)
        {
            return *this;
        }

        _maturity = source._maturity;
        _strike = source._strike;
        _type = source._type;
        _exercise = source._exercise;

        return *this;
    }

    /// @brief Destructor
    Payoff::~Payoff() {}

    /// @brief return maturity
    /// @return maturity
    double Payoff::getMaturity() const
    {
        return _maturity;
    }

	/// @brief return strike
    /// @return strike
    double Payoff::getStrike() const
    {
        return _strike;
    }

	/// @brief return type
    /// @return type
    Payoff::Type Payoff::getType() const
    {
        return _type;
    }

	/// @brief return exercise
    /// @return exercise
    Payoff::Exercise Payoff::getExercise() const
    {
        return _exercise;
    }
}