from deserialize import deserialize_raw
import os
import sys

def main():
    if (len(sys.argv) > 1):        
        path = os.path.join(os.getcwd(),sys.argv[1])
        print(path)
    else:
        print("Input not specified. Using default")
        path = "c:/Users/jeffr/Documents/SimpleIDE/My Projects/TUTagSat2021/test/CoolTerm Capture 2021-04-01 17-02-30.txt"
    frame = deserialize_raw(path)
    sorted = frame.sort_values(["FN","PC","VIDX"])
    if (len(sys.argv) > 2):
        sorted.to_csv(sys.argv[2])
    else:
        print(sorted.to_string())

main()