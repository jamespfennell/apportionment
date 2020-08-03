"""
Python script to compare priorty values from 
the Census Bureau with those from the program
"""
import csv
import math
import sys

with open(sys.argv[1]) as g:
    program_csv = csv.DictReader(sys.stdin)
    bureau_csv = csv.DictReader(g)
    for program_row, bureau_row in zip(program_csv, bureau_csv):
        assert int(program_row["house_seat"]) == int(bureau_row["house_seat"])
        assert program_row["entity"] == bureau_row["state"]
        assert int(program_row["entity_seat"]) == int(bureau_row["state_seat"])
        if round(float(program_row["priority_value"])) != int(bureau_row["priority_value"]):
            if abs(float(program_row["priority_value"]) - int(bureau_row["priority_value"])) <= 1:
                print("Warning: priority value is off, but by less than 1, indicating a rounding error")
                print("Program data", program_row)
                print("Census bureau data", bureau_row)
                continue
        assert round(float(program_row["priority_value"])) == int(bureau_row["priority_value"])
