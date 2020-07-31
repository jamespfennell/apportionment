# Apportionment using the Huntington–Hill method

This is a program for calculating apportionment in a representive body using the Huntington-Hill method.
This method is currently used for apportioning the United States House of Representives.
Moreover, with an additional assignment of two seats per State and three for the District of Columbia,
    this method also determines the apportionment of the US Electoral College.

The program takes an input CSV containing populations and
By default, the program runs like a classic Unix utility, 
    reading an input CSV containing populations from stdin, 
    and printing an output CSV containing apportionments to stdout:

    cat 2010_populations.csv | apportionment > 2010_apportionment.csv

It can also be run like a regular program with input and output files specified:

    apportionment -i 2010_populations.csv -o 2010_apportionment.csv

The input CSV must have two columns, the first containing the names of the entities amongst which seats are being apportioned,
    and the second the associated populations.
The output CSV, by default, contains the names and the apportionments assuming 435 seats
    (i.e., the number of seats in the US House).
The number of seats to apportion can be changed using the `--min` and `--max` arguments;
    if `min` and `max` are both provided and not the same, the resulting CSV will containing the apportionments
    for _all_ seat sizes between the two bounds.

Alternatively, the program can output a "seat by seat" CSV file
    which records each seat assigned in the order it was assigned.
This is done by passing the `--seat-by-seat` command line argument.
In this case, the output CSV contains four columns:
    the row number, 
    the name of the entity to which this seat was assigned,
    the priority number of the entity at the time this seat was assigned,
    and the number of seats so far that this entity has been assigned.

## Verifying historical US House apportionments

The Huntington-Hill method has been in use since the apportionment following the 1950 census.
As of time of writing (2020), it has been used for seven reapportionments.
This program can, and has, been used to verify these apportionments.

Explain the data layout..

Current guesses...

When verifying historical apportionments, the following are important to note.

- The Census Bureau has sometimes included overseas US Government 
    employees in the population counts used for apportioning.
    The Census Bureau did this in 1970 and from 1990 to the present.
    In 2010, this did not change the apportionment, but it did in
    2000 (North Carolina was apportioned the 435th seat, instead of Utah)
    and in 1990 (Washington, instead of Massachusetts).
    The constitutionally of this practice was upheld by the Supreme Court in ._Franklin v. Massachusetts_.

- The Census Bureau uses floating point arithmetic when calculating 
    the priority values of each state at each point in the algorithm.
    Using lossy floating point arithmetic is not neccessary and
    this application generally avoids it.
    Nevertheless, looking at the priority numbers, we see no reason why 
    the results should be different because the priority values
    are sufficiently far apart to not be suceptible to floating point round-off errors.



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

