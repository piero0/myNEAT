#!/usr/bin/python3

from enum import Enum
import os
import subprocess as sp

class IO:
    def __init__(self):
        self.logFile = "../neat.log"
        self.gvDir = "graphs"
        self.imgDir = "images"

    def loadLogs(self):
        print("Loading log file...")
        data = []
        try:
            with open(self.logFile) as f:
                data = f.readlines()
        except(IOError):
            print("Error: Could open log file")
        return data

    def saveGV(self, filename, data):
        try:
            with open(os.path.join(self.gvDir, filename), "w") as f:
               f.write(data)
            pass
        except(IOError):
            print("Error: Could open file {0}".format(filename))
            return False
        return True

    def execGraphviz(self, gv, img):
        cmdline = ["dot", "-Tpng", os.path.join(self.gvDir, gv), "-o", os.path.join(self.imgDir,img)]
        sp.run(cmdline)

class Mode(Enum):
    START = 0
    GENOMES = 1
    GENES = 2

class ProgBar:
    @staticmethod
    def draw(cur, total):
        proc = cur/(total) * 100.
        num = int(proc/5)+1
        print("\r{0}/{1} [{2}] {3:.2f}%".format(cur, total, "#"*num, proc), end='')

class LogParser:
    def __init__(self):
        self.io = IO()
        self.mode = Mode.START
        self.epochs = [[]]
        self.genes = []
        self.sensors = 2
        self.outputs = 1

    def line2genome(self, line):
        return line.split("|")[1:-1]

    def line2gene(self, line):
        return line.replace("-", " ").split(" ")[1:]

    def entry2graph(self, entry):
        return "{0}->{1} [label={2:.2f}];\n".format(entry[0], entry[1], float(entry[2]))

    def genome2graph(self, genome, filename):
        out = "digraph {0}".format(filename)
        out += " {\nrankdir=BT\n{rank=source;"

        #sources
        out += ";".join([str(idx) for idx in range(self.sensors)])

        out += "}\n{rank=sink;"

        #sinks
        out += ";".join([str(idx) for idx in range(self.sensors, self.sensors+self.outputs)])

        out += "}\n"

        #hidden
        for idx in genome:
            out += self.entry2graph(self.genes[int(idx)])

        out += "}"

        return out

    def parseLogs(self, data):
        print("Parsing log file...")
        epochIdx = -1
        datalen = len(data)-1

        for idx, line in enumerate(data):
            line = line.split(" ", 2)[2].strip()
            ProgBar.draw(idx, datalen)

            if line == "Epoch dump":
                epochIdx+=1
                self.epochs.append([])
                self.mode = Mode.GENOMES
                continue

            if line.startswith("|") and self.mode == Mode.GENOMES:
                self.epochs[epochIdx].append(self.line2genome(line))

            if line.startswith("Master genome"):
                self.mode = Mode.GENES
                continue

            if self.mode == Mode.GENES:
                self.genes.append(self.line2gene(line))
        print()

    def saveGVs(self):
        print("Saving graphviz files...")
        epNum = len(self.epochs)-1
        gNum = len(self.epochs[0])-1

        for eidx, epoch in enumerate(self.epochs):
            for gidx, genome in enumerate(epoch):
                filename = "e{0}g{1}".format(eidx, gidx)
                gvData = self.genome2graph(genome, filename)
                if not self.io.saveGV(filename + ".gv", gvData):
                    print("Error: Could not save {0}".format(filename))
                ProgBar.draw(eidx*gNum + gidx, epNum*gNum)
        print()

    def convertFiles(self):
        print("Converting gvs to imgs ...")
        files = os.listdir(self.io.gvDir)
        total = len(files)-1

        for idx, f in enumerate(files):
            ProgBar.draw(idx, total)
            self.io.execGraphviz(f, "{0}.png".format(f[:-3]))

        print()

    def main(self):
        print("### NEAT log -> Graphviz ###")
        data = self.io.loadLogs()
        if(len(data) == 0):
            return
        self.parseLogs(data)
        self.saveGVs()
        self.convertFiles()
        print("Done")

if __name__ == "__main__":
    LogParser().main()
