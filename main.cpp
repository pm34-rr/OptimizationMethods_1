#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Interval
{
	double left;
	double right;
};

Interval beginInterval{ -10, 20 };

double EPS;

double f( double x )
{
	return x * x + 2 * x - 4;
}

void printMinHead( ofstream & output )
{
	output << "Iter" << '\t' << "Left" << '\t' << "Right" << '\t' << "Delta" << '\t' << "Relation" << endl;
}

void printMinStep( ofstream & output, int k, const Interval & current, const Interval & prev )
{
	output.precision(7);
	output << std::scientific;
	output << k << '\t' << current.left << '\t' << current.right << '\t';
	double currentDelta = current.right - current.left;
	double relation = (prev.right - prev.left) / currentDelta;
	output << currentDelta << '\t' << relation << endl;
}

void printIntervalHeadFixX( ofstream & output )
{
	output << "Delta\t" << "Interval\t\t" << "Length\t" << "Iter count" << endl;
}

void printIntervalHeadFixEps( ofstream & output )
{
	output << "x0\t" << "Interval\t" << "Length\t" << "IterCount" << endl;
}

void printIntervalStep( ofstream & output, Interval & interval, int iterCount, double delta, double x, bool needX )
{
	output.precision( 7 );
	output << std::scientific;
	output << (needX ? x : delta);
	output << '\t' << "[" << interval.left << "," << interval.right << "]\t" << interval.right - interval.left << "\t" << iterCount << endl;
}

double dichotomy( int & iterCount )
{
	static int count = 0;
	--count;
	ofstream output("dichotomy" + to_string( count ) + ".txt");

	int k = 0;
	Interval currentInterval	= beginInterval;
	Interval prevInterval		= currentInterval;
	double x1, x2;

	printMinHead( output );
	double DELTA = EPS / 2.0;
	while (currentInterval.right - currentInterval.left >= EPS) {
		prevInterval = currentInterval;

		x1 = (currentInterval.left + currentInterval.right - DELTA) / 2;
		x2 = (currentInterval.left + currentInterval.right + DELTA) / 2;
		if (f(x1) < f(x2))
			currentInterval.right = x2;
		else
			currentInterval.left = x1;

		++k;
		printMinStep( output, k, currentInterval, prevInterval );
	}
	iterCount = k;
	return ( currentInterval.left + currentInterval.right ) * 0.5;
}

double goldenSection( int & iterCount )
{
	static int count = 0;
	--count;

	ofstream output( "goldenSection" + to_string( count ) + ".txt" );

	int k = 0;
	Interval currentInterval	= beginInterval;
	Interval prevInterval		= currentInterval;
	double x1, x2;
	double f1, f2;

	const double FIRST	= ( 3.0 - sqrt( 5.0 ) ) / 2.0;
	const double SECOND = ( sqrt( 5.0 ) - 1.0 ) / 2.0;
	int way = 0;
	printMinHead( output );
	while ( currentInterval.right - currentInterval.left >= EPS ) {
		prevInterval = currentInterval;

		if (way == -1) {
			x1 = x2;
			f1 = f2;

			x2 = currentInterval.left + SECOND * (currentInterval.right - currentInterval.left);
			f2 = f(x2);
		}
		else if ( way == 1 ) {
			x2 = x1;
			f2 = f1;

			x1 = currentInterval.left + FIRST * (currentInterval.right - currentInterval.left);
			f1 = f(x1);
		}
		else {
			x1 = currentInterval.left + FIRST * (currentInterval.right - currentInterval.left);
			f1 = f(x1);

			x2 = currentInterval.left + SECOND * (currentInterval.right - currentInterval.left);
			f2 = f(x2);
		}

		if (f1 < f2) {
			currentInterval.right = x2;
			way = 1;
		}
		else {
			currentInterval.left = x1;
			way = -1;
		}
		++k;

		printMinStep(output, k, currentInterval, prevInterval);
	}
	iterCount = k;
	return ( currentInterval.left + currentInterval.right ) * 0.5;
}

void createFibArray( vector<double> & array, int & iterCount )
{
	const double END_VALUE = ( beginInterval.right - beginInterval.left ) / EPS;
	double f1, f2, f3;
	f1 = f2 = f3 = 1;
	int iter = 0;
	while ( END_VALUE >= f3 ) {
		f3 = f1 + f2;
		f1 = f2;
		f2 = f3;
		++iter;
	}
	array.push_back( f3 - f1 ); // f1
	array.push_back( f1 );		// f2
	array.push_back( f3 );		// f3

	iterCount = iter;
}

double fibonacci( int & iterCount )
{
	static int count = 0;
	--count;

	ofstream output( "fibonacci" + to_string( count ) + ".txt" );

	const double DELTA = (beginInterval.right - beginInterval.left);
	Interval currentInterval = beginInterval;
	Interval prevInterval	 = currentInterval;

	vector<double> fibNums;
	int n;
	createFibArray( fibNums, n );

	double max = fibNums[2];
	double x1, x2;
	x1 = beginInterval.left + fibNums[0] / max * DELTA;
	x2 = beginInterval.left + fibNums[1] / max * DELTA;
	fibNums.clear();

	double f1, f2;
	f1 = f( x1 );
	f2 = f( x2 );

	bool left;
	if ( f1 < f2 ) {
		currentInterval.right = x2;
		left = true;
	}
	else {
		currentInterval.left = x1;
		left = false;
	}

	int k = 1;
	printMinHead( output );
	printMinStep( output, k, currentInterval, prevInterval );
	while ( k != n ) {
		prevInterval = currentInterval;
		if ( left ) {
			x2 = x1;
			x1 = currentInterval.left + ( currentInterval.right - x2 );
			f2 = f1;
			f1 = f( x1 );
		}
		else {
			x1 = x2;
			x2 = currentInterval.right - ( x1 - currentInterval.left );
			f1 = f2;
			f2 = f( x2 );
		}

		if ( f1 < f2 ) {
			currentInterval.right = x2;
			left = true;
		}
		else {
			currentInterval.left = x1;
			left = false;
		}
		++k;

		printMinStep( output, k, currentInterval, prevInterval );
	}
	iterCount = k;
	return ( currentInterval.left + currentInterval.right ) * 0.5;
}

void minInterval( double x0, ofstream & output, bool needX )
{
	double h;
	if ( f( x0 ) > f( x0 + EPS ) )
		h = EPS;
	else if ( f( x0 ) > f( x0 - EPS ) )
		h = -EPS;

	double prevX = x0;
	double currentX = prevX + h;

	int k = 1;
	Interval currentInterval{ x0, x0 };
	double prevF = f( prevX );
	double currentF = f( currentX );
	while ( prevF > currentF ) {
		currentInterval.right = prevX;
		prevX = currentX;
		prevF = currentF;
		h *= 2;
		currentX = prevX + h;
		currentF = f( currentX );
		currentInterval.left = currentX;
		++k;
	}
	printIntervalStep( output, currentInterval, k, EPS, x0, needX );
}

void printResultHead( ofstream & output )
{
	output << "EPS\t" << "IterCount\t" << "Error\t" << "Result" << endl;
}

int main()
{
	double TRUE_VALUE = -1.0;

	ofstream output( "dichotomy_result.txt" );
	printResultHead( output );
	output.precision( 14 );
	output << std::scientific;
	for ( EPS = 1e-1; EPS > 1e-14; EPS *= 1e-1 ) {
		int iterCount;
		double x = dichotomy( iterCount );
		output << EPS << ' ' << iterCount << ' ' << fabs( x - TRUE_VALUE ) / TRUE_VALUE << ' ' << x << endl;
	}
	output.close();

	output.open( "goldenSection_result.txt" );
	printResultHead( output );
	output.precision( 14 );
	output << std::scientific;
	for ( EPS = 1e-1; EPS > 1e-14; EPS *= 1e-1 ) {
		int iterCount;
		double x = goldenSection( iterCount );
		output << EPS << ' ' << iterCount << ' ' << fabs( x - TRUE_VALUE ) / TRUE_VALUE << ' ' << x << endl;
	}
	output.close();

	output.open( "fibonacci_result.txt" );
	printResultHead( output );
	output.precision( 14 );
	output << std::scientific;
	for ( EPS = 1e-1; EPS > 1e-14; EPS *= 1e-1 ) {
		int iterCount;
		double x = fibonacci( iterCount );
		output << EPS << ' ' << iterCount << ' ' << fabs( x - TRUE_VALUE ) / TRUE_VALUE << ' ' << x << endl;
	}
	output.close();

	output.open( "interval_fixX.txt" );
	printIntervalHeadFixX( output );
	for ( EPS = 1e-1; EPS > 1e-14; EPS *= 1e-1 )
		minInterval( 4.0, output, false );
	output.close();

	output.open( "interval_fixDelta.txt" );
	printIntervalHeadFixEps( output );
	EPS = 1e-8;
	for ( double x = -95; x < 100; x += 10.0 )
		minInterval( x, output, true );
	output.close();

	return 0;
}
