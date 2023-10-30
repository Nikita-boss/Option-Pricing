// Implementation of the header file NumericalEuropeanEngine.hpp

#include "NumericalEuropeanEngine.hpp"

namespace PricingLibrary {

	/// @brief Default constructor
	/// @param S underlying price
	/// @param sigma volatility
	/// @param r risk-free rate
	/// @param b cost-of-carry parameter 
	NumericalEuropeanEngine::NumericalEuropeanEngine(double S, double sigma, double r, double b)
	: _S{S}, _sigma{sigma}, _r{r}, _b{b}, _h{0.01} {}

	/// @brief Copy constructor
	/// @param source AnalyticAmericanPerpetualEngine object
	NumericalEuropeanEngine::NumericalEuropeanEngine(const NumericalEuropeanEngine& source)
	: PricingEngine{source}, _S{source._S}, _sigma{source._sigma}, _r{source._r}, _b{source._b} 
	{}

	/// @brief Copy assignemnt
	/// @param source AnalyticEuropeanEngine object
	/// @return AnalyticEuropeanEngine object
	NumericalEuropeanEngine& NumericalEuropeanEngine::operator= (const NumericalEuropeanEngine& source)
	{
		// check for self-assignment
		if (this == &source)
		{
			return *this;
		}

		PricingEngine::operator=(source);
		_S = source._S;
		_sigma = source._sigma;
		_r = source._r;
		_b = source._b;

		return *this;
	}

	/// @brief Default destructor
	NumericalEuropeanEngine::~NumericalEuropeanEngine() {}

	/// @brief Validate if engine was passed to a correct option
	/// @param exercise American or European
	void NumericalEuropeanEngine::validate(const Payoff::Exercise& exercise) const
	{
		if (exercise != Payoff::European)
		{
		throw IncorrectEngineException("Only European Options have analytic solution.");
		}
	}

	/// @brief return delta greek
	/// @param payoff Payoff object
	/// @return delta
	double NumericalEuropeanEngine::getEngineDelta(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		if (payoff->getType() == Payoff::Type::Call)
		{
			return getEngineCallDelta(payoff);
		}
		else
		{
			return getEnginePutDelta(payoff);
		}
	}

	/// @brief return gamma greek
	/// @param payoff Payoff object
	/// @return gamma
	double NumericalEuropeanEngine::getEngineGamma(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		if (payoff->getType() == Payoff::Type::Call)
		{
			return getEngineCallGamma(payoff);
		}
		else
		{
			return getEnginePutGamma(payoff);
		}
	}

	/// @brief analytic exact call price
	/// @param payoff Payoff object
	/// @param S_new underlying price
	/// @return price
	double NumericalEuropeanEngine::getAnalyticCallPrice(const std::shared_ptr<Payoff>& payoff, double S_new) const
	{
		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};
		boost::math::normal_distribution standard_normal(0.0, 1.0);
		double d1 = ( std::log(S_new / K) + (_b + _sigma * _sigma / 2) * T ) / (_sigma * std::sqrt(T));
		double d2 = d1 - _sigma * std::sqrt(T);
		double N_d1 = boost::math::cdf(standard_normal, d1);
		double N_d2 = boost::math::cdf(standard_normal, d2);
		double price = S_new * std::exp( (_b - _r) * T ) * N_d1 - K * std::exp(-_r * T) * N_d2;

		return price;
	}

	/// @brief analytic exact put price
	/// @param payoff Payoff object
	/// @param S_new underlying price
	/// @return price
	double NumericalEuropeanEngine::getAnalyticPutPrice(const std::shared_ptr<Payoff>& payoff, double S_new) const
	{
		double K{payoff->getStrike()};
		double T{payoff->getMaturity()};
		boost::math::normal_distribution standard_normal(0.0, 1.0);
		double d1 = ( std::log(S_new / K) + (_b + _sigma * _sigma / 2) * T ) / (_sigma * std::sqrt(T));
		double d2 = d1 - _sigma * std::sqrt(T);
		double N_d1 = boost::math::cdf(standard_normal, -d1);
		double N_d2 = boost::math::cdf(standard_normal, -d2);
		double price = -S_new * std::exp( (_b - _r) * T ) * N_d1 + K * std::exp(-_r * T) * N_d2;

		return price;
	}

	/// @brief call delta greek
	/// @param payoff Payoff object
	/// @return delta
	double NumericalEuropeanEngine::getEngineCallDelta(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double price_plus_h = getAnalyticCallPrice(payoff, _S + _h);
		double price_minus_h = getAnalyticCallPrice(payoff, _S - _h);
		double delta = (price_plus_h - price_minus_h) / (2 * _h);

		return delta;
	}

	/// @brief call gamma greek
	/// @param payoff Payoff object
	/// @return gamma
	double NumericalEuropeanEngine::getEngineCallGamma(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double price = getAnalyticCallPrice(payoff, _S);
		double price_plus_h = getAnalyticCallPrice(payoff, _S + _h);
		double price_minus_h = getAnalyticCallPrice(payoff, _S - _h);
		double gamma = (price_plus_h - 2 * price + price_minus_h) / (_h * _h);

		return gamma;
	}

	/// @brief put delta greek
	/// @param payoff Payoff object
	/// @return delta
	double NumericalEuropeanEngine::getEnginePutDelta(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double price_plus_h = getAnalyticPutPrice(payoff, _S + _h);
		double price_minus_h = getAnalyticPutPrice(payoff, _S - _h);
		double delta = (price_plus_h - price_minus_h) / (2 * _h);

		return delta;
	}

	/// @brief put gamma greek
	/// @param payoff Payoff object
	/// @return gamma
	double NumericalEuropeanEngine::getEnginePutGamma(const std::shared_ptr<Payoff>& payoff) const
	{
		validate(payoff->getExercise());

		double price = getAnalyticPutPrice(payoff, _S);
		double price_plus_h = getAnalyticPutPrice(payoff, _S + _h);
		double price_minus_h = getAnalyticPutPrice(payoff, _S - _h);
		double gamma = (price_plus_h - 2 * price + price_minus_h) / (_h * _h);

		return gamma;
	}

	/*! \warning Not implemented calculation of option price */
	double NumericalEuropeanEngine::getEnginePrice(const std::shared_ptr<Payoff>& payoff) const
	{
		return -1.0;
	}
	
	/*! \warning Not implemented calculation of Theta greek */
	double NumericalEuropeanEngine::getEngineTheta(const std::shared_ptr<Payoff>& payoff) const
	{
		return -1.0;
	}

	/*! \warning Not implemented calculation of Vega greek */
	double NumericalEuropeanEngine::getEngineVega(const std::shared_ptr<Payoff>& payoff) const
	{
		return -1.0;
	}
}