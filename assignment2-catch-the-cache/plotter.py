import matplotlib.pyplot as plt
import argparse
import numpy as np

def process_file(filename:str):
    with open(filename,'r') as f:
        f.readline()
        f.readline()
        f.readline()
        size = []
        cycles = []
        for line in f.readlines():
            line = line.strip()
            size.append(float(line.split()[0]))
            cycles.append(float(line.split()[-1]))
            
        plt.plot(np.linspace(start=min(size),stop=max(size),num=len(size)),cycles)
        plt.xlabel("Size of buffer")
        plt.ylabel("Time in cycles")    
        plt.title("Cache size measurement")

        plt.savefig("time_vs_size_of_buffer.png",dpi=400)
        


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--filename")
    args = parser.parse_args()

    filename = args.filename

    process_file(filename)

    