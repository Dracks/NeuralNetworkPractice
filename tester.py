import os
import subprocess

def execute(dessign, output, learningFile, epochs, overtraining, learningRate, momentum, predictFile):
    call="./build/p1 -N "+dessign+" -L "+learningFile+" -e "+epochs+" -o "+overtraining+" -l "+learningRate+" -m "+momentum+" -D "+output+".dump -P "+predictFile+" -W "+output+".pred -n"
    #print "Call:", call
    fileOutput=output+".stdout"
    #print "Output:", fileOutput, " Errors:", fileErr
    subprocess.call(call, shell=True, stdout=open(fileOutput, 'w'));

if __name__ == '__main__':
	fileLearning="dataForTraining.txt"
	fileTest="dataForOutput.txt"
	outputFolder="results/"
	
	epochsList=["0200", "0500", "1000", "2000"]
	learningRateList=["0.10", "0.20", "0.30", "0.35" ,"0.40", "0.45"]
	momentumList=["0.10", "0.20", "0.30", "0.35", "0.40", "0.45" ,"0.50"]
	networkDessignList=["4,5,1", "4,8,1", "4,5,3,1"]

	for networkDessign in networkDessignList:
		for epochs in epochsList:
			for learningRate in learningRateList:
				for momentum in momentumList:
					file=outputFolder+"n"+networkDessign+"e"+epochs+"l"+learningRate+"m"+momentum
					execute(networkDessign, file, fileLearning, epochs, "0", learningRate, momentum, fileTest)
					
