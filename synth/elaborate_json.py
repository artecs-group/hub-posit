import csv
import sys

if __name__=="__main__":
	fpath = sys.argv[1]
	# design = sys.argv[2]
	design = '_'.join(sys.argv[2].split('/')[-2:])
	# Read values
	with open(fpath+"/area.csv", mode ="r")as area_file:
		with open(fpath+"/power.csv", mode ="r")as power_file:
			with open(fpath+"/delay.csv", mode ="r")as delay_file:
				area_csv  = list(csv.reader(area_file) )
				power_csv = list(csv.reader(power_file))
				delay_csv = list(csv.reader(delay_file))

	assert (len(area_csv) == len(power_csv) == len(delay_csv))
	lines = len(area_csv)

	# Write data in JSON
	with open(fpath+"/synopsys_results.json", mode ="w")as o_file:
		o_file.write("[\n")
		for i in range(lines):
			o_file.write("\t{\n")
			o_file.write("\t\t\"Design\": \""+design+"\",\n")
			o_file.write("\t\t\"Bits\": "+area_csv[i][0]+",\n")
			o_file.write("\t\t\"Freq\": "+area_csv[i][1]+",\n")
			o_file.write("\t\t\"Slack\": \""+delay_csv[i][3]+"\",\n")
			o_file.write("\t\t\"Area\": "+area_csv[i][2]+",\n")
			o_file.write("\t\t\"Power\": "+power_csv[i][2]+",\n")
			o_file.write("\t\t\"Delay\": "+delay_csv[i][2]+",\n")
			o_file.write("\t\t\"Energy\": "+str(float(power_csv[i][2])*float(delay_csv[i][2])*1000)+",\n")	# PDP
			o_file.write("\t\t\"ADP\": "+str(float(area_csv[i][2])*float(delay_csv[i][2]))+"\n")
			# Write last entry without comma
			o_file.write("\t}\n") if i == lines-1 else o_file.write("\t},\n")
		o_file.write("]")
