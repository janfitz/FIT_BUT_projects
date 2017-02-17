#!/usr/bin/env python
# -*- coding: utf-8 -*-

#Downloading records from IR
#Author: Jan Fitz
#Date: 2015-07-31

#--------------------------------------------------

#Import all libraries
import urllib2 #library for opening the downloaded page
import codecs #library for using ut-8 textcoding
import csv
from bs4 import BeautifulSoup #import the main library - Beautifulsoup for parsing html content

def checkRecords():
		
		#Tell user about the curent operation
		print ("Preparing subjects control ...")

		#Open file for writing only
		f = codecs.open("ir_records.txt", "w", "utf-8")

		recordsCounter = 0
		foundCounter = 0

		#Function for checking RC records
		with open('rc.csv', 'rb') as csvfile:
			seznam_rc = csv.reader(csvfile, delimiter=',', quotechar='|')
			for row in seznam_rc:

				rc = row[0]

				if rc:
					print ("\tChecking RC: "+str(rc))

					recordsCounter = recordsCounter + 1

					#Set target for Beautifulsoup library 
					soup = BeautifulSoup(urllib2.urlopen('http://insolvencni-rejstrik.cz/?sg=1&nazev_osoby=&ic=&jmeno=&rc='+str(rc)+'&mesto=&datum_narozeni=&bc_vec=&rocnik=&id_osoby_puvodce=&zahajeno_od=&zahajeno_do=&druh_stav_rizeni=').read())

					#Count amount of pages
					if (soup.find("table", {"id": "results"})):
						print ("\t\tFound record for RC: "+str(rc))
						f.write("Found record for RC: "+str(rc)+"\n")
						foundCounter = foundCounter + 1

				else:
					pass

		#Function for checkig ICO records
		with open('rc.csv', 'rb') as csvfile:
			seznam_ico = csv.reader(csvfile, delimiter=',', quotechar='|')
			for row in seznam_ico:

				ico = row[1]

				if ico:
					print ("\tChecking for ICO: "+str(ico))

					recordsCounter = recordsCounter + 1

					#Set target for Beautifulsoup library 
					soup = BeautifulSoup(urllib2.urlopen('http://insolvencni-rejstrik.cz/?sg=1&nazev_osoby=&ic='+str(ico)+'&jmeno=&rc=&mesto=&datum_narozeni=&bc_vec=&rocnik=&id_osoby_puvodce=&zahajeno_od=&zahajeno_do=&druh_stav_rizeni=').read())

					#Count amount of pages
					if (soup.find("table", {"id": "results"})):
						print ("\t\tFound record for ICO: "+str(ico))
						f.write("Found record for ICO: "+str(ico)+"\n")
						foundCounter = foundCounter + 1

				else:
					pass

		#Tell user about the curent operation
		print ("Task Complete! "+str(recordsCounter)+" were checked and "+str(foundCounter)+" records were found.")
		print ("Foudn records were saved in ir_records.txt")

		#Close file 
		f.close()

#Run main function checkRecords
checkRecords()
