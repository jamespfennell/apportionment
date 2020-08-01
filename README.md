# Apportionment using the Huntington–Hill method

This is a program for calculating apportionment in a representive body using the Huntington-Hill method.
This method is currently used for apportioning the United States House of Representives.
Moreover, with an additional assignment of two seats per State and three for the District of Columbia,
    this method also determines the apportionment of the US Electoral College.

The program takes an input CSV containing populations and outputs a CSV containing apportionments:

    apportionment --input data/2010_census_data.csv --state-column state --pop-column pop_total

The input CSV must have two columns, one containing the names of the states
     amongst which seats are being apportioned,
    and the second the associated populations.
These two columns may be specified by name as above; if names are ommited,
    the first and second column are used respectively.

The output CSV, by default, contains the names and the apportionments assuming 435 seats
    (i.e., the number of seats in the US House).
The number of seats to apportion can be changed using the `--min` and `--max` arguments;
    if `min` and `max` are both provided and not the same, the resulting CSV will containing the apportionments
    for _all_ seat sizes between the two bounds.

Alternatively, the program can output a "seat by seat" CSV file
    which records each seat assigned in the order it was assigned.
This is done by passing the `--seat-by-seat` command line argument.
The output CSV in this case contains four columns:
    the total number of seats that have been apportioned so far,
    the name of the state to which this seat was assigned,
    the priority number of the state at the time this seat was assigned,
    and the number of seats so far that this state has been assigned.

## Verifying historical US House apportionments

The Huntington-Hill method has been used for the US House
     since the apportionment following the 1940 census.
At time of writing (2020), it has been used for eight apportionments.
The continouous integration pipeline for this repository
   verifies these eigth apportionments using Census Bureau population.
Moreover, for the three most recent apportionments the priority values
    of all 385 apportioned seats are also verified.

Explain the data layout..

Current guesses...

When verifying historical apportionments, keep in mind the following.

- The Census Bureau has sometimes included overseas US Government 
    employees in the population counts used for apportioning.
    They did this in 1970 and from 1990 to the present.
    In every year except 2010, this has changed the apportionment:
    in 1970, Oklahoma was apportioned the 435th seat instead of Connecticut;
    in 1990, Washington instead of Massachusetts;
    and in 2000, North Carolina instead of Utah.
    The constitutionally of this practice was upheld by the Supreme Court in Franklin v. Massachusetts.

- The Census Bureau uses floating point arithmetic when calculating 
    the priority values of each state at each point in the algorithm.
    Using lossy floating point arithmetic is problematical because
    (a) it makes it harder to precisely replicate the 
        algorithm because the priority values can be 
        different depending on the precision in use, and
    (b) in certain _highly unlikely_ scenarios the calculated apportionment
        may be incorrect because of rounding errors.
    This application does not in general use floating point arithmetic
    but rather solves the problem exactly.

## What does solving the problem exactly mean?

The Huntington–Hill method does not require the priority values to be explicitly calculated.
To select the state with the largest priority value, it is only necessary to 
be able to _compare_ priority values and choose the larger one.
Given two priority values of the form 
`P1 / sqrt(n1 * (n1 + 1))` and
`P2 / sqrt(n2 * (n2 + 1))`, the first is larger if and only if:

    P1 * sqrt(n2 * (n2 + 1)) > P2 * sqrt(n1 * (n1 + 1))

Squaring both sides, this is true if and only if:

    P1 * P1 * n2 * (n2 + 1) > P2 * P2 * n1 * (n1 + 1)

All of the numbers in the inequality here are integers, and
so it is possible to detemine the larger priority number using integer arithmetic alone.

The single catch is that this program is written in C++ using 64 bit integers,
and for certain large values of `P` and `n` it may not be possible
to calculate both sides of the inequality.
In this case (which never arises in the US House case),
the application reverts to floating point arithmetic and logs a warning.


## Building the program

Checkout the Git repo and run `make`.
The program can be found at `bin/apportionment`.

To build using an alternative compiler...

To run the tests...


## C++ modules within

The program is written using two C++ modules with public headers, 
    meaning you can use these modules in other C++ programs if you like.
One module contains the apportionment algorithm and related data structures;
    the other is a bare-bones (but templated) implementation of a max heap.
The use of a heap in the algorithm means that it runs in `O((n+m)log(n))`,
    where `n` is the number of entities/states and `m` is the number of seats to apportion.

(I don't know if this is the optimal running time.
If one can determine the priority number of the last seat assigned,
it's definitely possible to calculate the apportionment in `O(n)`.
However, can the last priority number can be calculated in less than `O(n+m)`? I have a
sneaking suspicion it may be possible.)

