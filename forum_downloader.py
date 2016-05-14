#!/usr/bin/env python
# -*- coding: utf-8 -*-

#Projekt ISJ 2014/2015
#Downloading data (all threads) from selected forum
#forum: http://forum.fedora.cz/

#Author: Jan Fitz, xfitzj00@stud.fit.vutbr.cz
#Date: 2015-04-20

#--------------------------------------------------

#Import all libraries
import sys #library for parsing terminal params
import urllib2 #library for opening the downloaded page
import codecs #library for using ut-8 textcoding
import fileinput #library for deleting lines with specified string
import StringIO #library for deleting lines containing specified string
import re #library for using regexes
import os #library for checking file existence
from bs4 import BeautifulSoup #import the main library - Beautifulsoup for parsing html content

#Funciton for checking an amount of forum messages
def messagesAmount():

        messageCounter = 0
        
        #Set target for Beautifulsoup library on main forum page
        soup = BeautifulSoup(urllib2.urlopen('http://forum.fedora.cz/').read())

        #Load the main table with sections
        AllTable = soup.find("table", {"class": "list"})

        #Loop for - load the hole table with all threads
        for messageNumber in AllTable.find_all("tr")[1:]:
                col = messageNumber.find_all("td")

                #Find the amount of messages in each thread
                messageAmount = str(col[2].text.strip())
                messageAmount = re.sub(" ","",messageAmount)         
                messageCounter = messageCounter + int(messageAmount)

        return messageCounter

def forumDownload():

        #Tell user about the curent operation
        print ("Preparing forum download ...")

        #Open file for writing only
        f = codecs.open("forum.txt", "w", "utf-8")

        #Set the counter of forum sections and pages
        sectionCounter = 1
        pageCounter = 1

        #Put amount of forum massages on the first line of forum.txt and separete them with "---"
        f.write(str(messagesAmount())+"\n")
        f.write("-------------------------------------------"+"\n")
        f.write("-------------------------------------------"+"\n \n")

        #While loop for browsing data in each section
        while sectionCounter != 7 + 1:    

                #Set target for Beautifulsoup library 
                soup = BeautifulSoup(urllib2.urlopen('http://forum.fedora.cz/list.php?'+str(sectionCounter)+',page=1').read())

                #Count amount of pages
                pageAmount = soup.find("div", {"class": "paging"})
                pageAmount = pageAmount.text.strip()
                pageAmount = pageAmount[0:13]
                pageAmount = pageAmount[::-1]
                pageAmount = pageAmount[0:2]
                pageAmount = pageAmount[::-1]
                pageAmount = int(pageAmount)

                #Tell user about curent operation
                section = soup.find("h1")
                print ("\t Downloading section"+" "+str(sectionCounter)+"/7"+" with "+str(pageAmount)+" pages ...")    

                #While loop for saving all data from thread into a file
                while pageCounter < pageAmount + 1:
                        
                        #Set target for Beautifulsoup library 
                        soup = BeautifulSoup(urllib2.urlopen('http://forum.fedora.cz/list.php?'+str(sectionCounter)+',page='+str(pageCounter)).read())
                        
                        #Load the main table with threads
                        AllTable = soup.find("table", {"class": "list"})

                        #Tell user about curent operation
                        section = soup.find("h1")
                        print ("\t \t Downloading page"+" "+str(pageCounter)+"/"+str(pageAmount)+" of section"+" \""+section.text+"\" ...")

                        #Loop for - load the hole table with all threads
                        for row in AllTable.find_all("tr")[1:]:
                                col = row.find_all("td")

                                #Find the name of each thread
                                name = col[1].a.text.strip()

                                #Separate link from thread name
                                namelink = col[1].find("a").get("href")

                                #Open each thread from previous links
                                soupThread = BeautifulSoup(urllib2.urlopen(namelink).read())

                                #Find the body of messages in threads
                                threadText = soupThread.find_all("div", {"class": "message"})
                                
                                for printingText in threadText:

                                        #Find message author name
                                        authorName = printingText.find("div", {"class": "message-author icon-user"})
                                        f.write("Autor: \t \t"+authorName.text.strip()+"\n\n")

                                        #Find message topic and date information
                                        threadInfo = printingText.find("small")
                                        infoResult = threadInfo.text.strip()
                                        #Replace three and more spaces
                                        totalResult = re.sub(' {3,1000}','Datum: \t \t',infoResult)
                                        f.write("Jmeno vlakna: \t"+totalResult+"\n\n")

                                        #Print number of page
                                        f.write("Strana: \t"+str(pageCounter)+". strana\n\n")

                                        #Print section name
                                        f.write("Sekce: \t \t"+section.text+"\n\n")

                                        #Find message text
                                        messageBody = printingText.find("div", {"class": "message-body"})
                                        result = messageBody.text.strip()               

                                        #Decode ascii code from czech characters
                                        u = 'Odpovědět'
                                        odpovedet = u.decode('utf-8')
                                        
                                        #Delete Citovat and Odpovedet from message body
                                        s = StringIO.StringIO(result)
                                        
                                        for line in s:
                                                if 'Citovat' not in line and odpovedet not in line:
                                                        f.write(line+"\n")

                                        #Separate all messages
                                        f.write("-------------------------------------------"+"\n")
                                        f.write("-------------------------------------------"+"\n \n")
                                        
                        #Increment page counter 
                        pageCounter = pageCounter + 1

                #Increment section counter 
                sectionCounter = sectionCounter + 1

                #Delete pageCounter variable
                pageCounter = 1

        #Delete all empty lines from file
        for line in fileinput.FileInput("forum.txt",inplace=1):
                if line.rstrip():
                        print line

        #Tell user about the curent operation
        print ("Task complete!")

        #Close file 
        f.close()

#If no param is set, forum downloading will be start
if (len(sys.argv) == 1):

        forumDownload()        

#If param -a is set, forum actuality will be check
elif (len(sys.argv) == 2) and (sys.argv[1] == "-a"):

        #Tell user about curent operation
        print ("Checking forum actuality ...")

        #Open file for writing only
        f = codecs.open("forum.txt", "r", "utf-8")        

        #Tell user about actual real amount forum messages
        print ("Amount of messages in forum is: "+str(messagesAmount()))

        #Print error if file is empty or doesnt exist
        try:
                if (os.stat('forum.txt').st_size == 0):      
                    sys.stderr.write("\t File doesnt exist or is empty! Fisrt you have to download forum content at least once. \n")
                    sys.exit(1)
        except OSError:
                 sys.stderr.write("No file")

        #Read the first line of forum.txt file which containg messages amount
        checkMessages = f.readline()

        #Compare messages amount in file forum.txt and actual forum messages amount
        if int(checkMessages) < int(messagesAmount()):
                question = raw_input('\t Forum isnt actual, do you want to update it? y/n: ')
                
                #Downloading will start
                if question == "y":

                        forumDownload() 

                #Downloading will not start
                elif question == "n":
                        print "OK, task complete without updating forum data! \n"

        #Forum is actual
        elif int(checkMessages) == int(messagesAmount()):
                print "\t Previous downloaded data are actual!"

        #Define error message
        else:
                sys.stderr.write("\t Something is wrong with checking forum data actuality! \n" )
                sys.exit(1) 

        #Close file 
        f.close()

#If wrong params combination is set, error will be printed out
else:
        sys.stderr.write("Script was started wrongly! You can use param \"-a\" for check forum actuality or nothing to start forum downloading. \n" )
        sys.exit(1)

