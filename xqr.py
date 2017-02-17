#!/usr/bin/env python
# -*- coding: utf-8 -*-
#XQR: Jan Fitz

#Parsing XML Document based on pseudo SQL language

#Author: Jan Fitz,
#Date: 2016-04-10

#--------------------------------------------------

import re #library for using regexes
import sys #system library
import argparse #library for parsing terminal params
import xml.dom.minidom #library for parsing xml
import codecs #library for using corect utf-8 file formating

counter = 0 #Variable for counting elements by LIMIT argument

#####################
# ARGUMENTS PARSING #
#####################

#If is set help argument and some other
if ('--help' in str(sys.argv) )and len(sys.argv) > 2:
    sys.stderr.write("Arguments error\n" )
    sys.exit(1)

#Print help
elif ('--help' in str(sys.argv) )and len(sys.argv) == 2:
    print  ("Skript provadi vyhodnoceni zadaneho dotazu, jenz je podobny prikazu SELECT")
    print  ("jazyka SQL, nad vstupem ve formatu XML. Vystupem je XML obsahujici elementy")     
    print  ("splnujici pozadavky dane dotazem.")                                               
    print  ("Pripustne parametry:")                                  
    print  ("       --help                 - vytiskne tuto napovedu")                           
    print  ("       --input=filename       - zadany vstupni soubor ve formatu XML")             
    print  ("       --output=filename      - zadany vystupni soubor ve formatu XML")                      
    print  ("       --qf=filename          - dotaz v dotazovacim jazyce definovanem  v souboru")                       
    print  ("       -n                     - negenerovat XML hlavicku na vystup skriptu")      
    print  ("       --root=element         - jmeno paroveho korenoveho elementu")
    sys.exit(0)

parser = argparse.ArgumentParser()
parser.add_argument("--input", help="input filename")
parser.add_argument("--output", help="output filename")
parser.add_argument("--query", help="pseudo SQL query")
parser.add_argument("--qf", help="input file with pseudo SQL query")
parser.add_argument("-n", help="do not generate XML header", action='store_true')
parser.add_argument("--root", help="optional name of root element placed in output file")
#Catch system exit - arguments error
if not parser.parse_args():
    sys.stderr.write("Arguments error\n" )
    sys.exit(1)
else:
    args = parser.parse_args()

#Checking incorect arguments - no query set
if not args.query and not args.qf:
    sys.stderr.write("Arguments error\n" )
    sys.exit(1)

#Both argumets queries are set
if args.query and args.qf:
    sys.stderr.write("Arguments error\n" )
    sys.exit(1)

#Query from file
if args.qf:
    file = open(args.qf, 'r')
    query_args = file.read()
    if query_args:
        query_args = query_args.split()
    else:
        sys.stderr.write("File with query is empty\n" )
        sys.exit(0)
#Query from argument
else:
    if args.query:
        query_args = args.query.split()
    else:
        sys.stderr.write("Arguments error\n" )
        sys.exit(1)

#####################
# CHECKING SEMANTIC #
#####################
try:
    query_len = len(query_args)

    if (query_args[0] != "SELECT") or (query_len < 2):    
        sys.stderr.write("Semantic error\n" )
        sys.exit(80)

    select_var = query_args[1]

    #If argument LIMIT is placed
    if query_args[2] == "LIMIT" and query_args[4] == "FROM": 

        limit_var = query_args[3]
        from_var = query_args[5]

        #Test if argumtnt WHERE is placed
        if query_len > 6:
            #Incorect anrgument 6
            if query_args[6] != "WHERE":
                sys.stderr.write("Semantic error\n" )
                sys.exit(80)
            else:
                where_var = query_args[7]
                relation_operator = query_args[8]
                literal = query_args[9]
        else:
            where_var = ""
            relation_operator = ""
            literal = ""

    #If no argument LIMIT is placed
    elif query_args[2] == "FROM": 

        from_var = query_args[3]
        limit_var = ""

        #only SELECT and FROM is set
        if query_len != 4:            
            #Test if argumtnt WHERE is placed
            if query_len > 4:
                #Incorect anrgument 4
                if query_args[4] != "WHERE":
                    sys.stderr.write("Semantic error\n" )
                    sys.exit(80)            
                else:
                    if query_len > 8:
                        sys.stderr.write("Semantic error\n" )
                        sys.exit(80)

                    where_var = query_args[5]
                    relation_operator = query_args[6]
                    literal = query_args[7]
            else:
                sys.stderr.write("Semantic error\n" )
                sys.exit(80)
        else:
            where_var = ""
            relation_operator = ""
            literal = "" 

    #Some other not specified option of arguments was set - error80
    else:
        sys.stderr.write("Semantic error\n" )
        sys.exit(80)

    #Chcek if literal contains "
    if literal:
        if re.match('\".*\"', literal):
            literal = re.findall(r'"([^"]*)"', literal)
            literal = literal[0]
            stringOrNumber = "string"
        elif literal.isdigit():
            stringOrNumber = "number"
        else:
            sys.stderr.write("Semantic error\n" )
            sys.exit(80)

    #Check maximum occurence of comma character
    if where_var.count('.') > 1:
        sys.stderr.write("Semantic error\n" )
        sys.exit(80)
    if from_var.count('.') > 1:
        sys.stderr.write("Semantic error\n" )
        sys.exit(80)

    #Check if limit_var is numeric
    if limit_var:
        if not limit_var.isdigit():
            sys.stderr.write("Semantic error\n" )
            sys.exit(80)

    #################
    # PROCESS QUERY #
    #################

    #If no input file was set - using stdin
    if not args.input:
        tmp = ""
        #Get all from stdin into string named tmp
        for line in sys.stdin:
            tmp = tmp + line

        dom = xml.dom.minidom.parseString(tmp)

    #Input file is set
    else:
        #Parse xml document
        dom = xml.dom.minidom.parse(args.input)

    #Output file was set
    if args.output:
        #Open file for writing only
        f = codecs.open(args.output, "w", "utf-8")  

        #If xml header have to be generate
        if not args.n:
            f.write('<?xml version="1.0" encoding="utf-8"?>')

        #If LIMIT is set to zero
        if limit_var:
            if int(limit_var) == 0:
                #Print specified root element and exit with no error
                if args.root:
                    f.write("<"+args.root+"/>")
                    sys.exit(0)
                #Only exit with no error
                else:
                    sys.exit(0)

        #If specific root element is specified - start element
        if args.root:
            f.write("<"+args.root+">")

        #We are selecting from ROOT
        if from_var == "ROOT":             
            for node in dom.getElementsByTagName(query_args[1]):
                #If LIMIT is set
                if limit_var:
                    if counter < int(limit_var):
                        #If WHERE is set
                        if where_var:
                            #We are selecting specified element
                            if where_var.startswith('.'):
                                attribute = ''.join(where_var.split('.')[1:])
                                if node.hasAttribute(attribute):
                                    #Case of CONTAINS in condition
                                    if relation_operator == "CONTAINS":
                                        if literal in node.attributes.item(0).value:
                                            f.write(node.toxml())
                                            counter += 1
                                    #Case of equal operator in condition
                                    elif relation_operator == "=":
                                        if stringOrNumber == "string":
                                            if str(node.getAttribute(attribute)) == str(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                        elif stringOrNumber == "number":
                                            if float(node.attributes.item(0).value) == float(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                    #Case of less operator in condition
                                    elif relation_operator == "<":                            
                                        if stringOrNumber == "string":
                                            if str(node.getAttribute(attribute)) < str(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                        elif stringOrNumber == "number":
                                            if float(node.attributes.item(0).value) < float(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                    #Case of grater operator in condition
                                    elif relation_operator == ">":
                                        if stringOrNumber == "string":
                                            if str(node.getAttribute(attribute)) > str(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                        elif stringOrNumber == "number":
                                            if float(node.attributes.item(0).value) > float(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                    #Unknown relation operator was set
                                    else:
                                        sys.stderr.write("Bad relation operator\n" )
                                        sys.exit(80) 
                            elif "." in where_var:
                                #We are selecting specified elemnt
                                if not where_var.startswith('.'):
                                    #Split element name and attribute name
                                    attribute = ''.join(where_var.split('.')[1:])
                                    element = where_var.split(".")[0]
                                    #Get all elements with specified attribute
                                    if node.hasAttribute(attribute):
                                        #Case of CONTAINS in condition
                                        if relation_operator == "CONTAINS":
                                            if literal in node.attributes.item(0).value:
                                                f.write(node.toxml())
                                        #Case of equal operator in condition
                                        elif relation_operator == "=":
                                            if stringOrNumber == "string":
                                                if str(node.getAttribute(attribute)) == str(literal):
                                                    f.write(node.toxml())
                                            elif stringOrNumber == "number":
                                                if float(node.getAttribute(attribute)) == float(literal):
                                                    f.write(node.toxml())
                                        #Case of less operator in condition
                                        elif relation_operator == "<":                            
                                            if stringOrNumber == "string":
                                                if str(node.getAttribute(attribute)) < str(literal):
                                                    f.write(node.toxml())
                                            elif stringOrNumber == "number":
                                                if float(node.getAttribute(attribute)) < float(literal):
                                                    f.write(node.toxml())
                                        #Case of grater operator in condition
                                        elif relation_operator == ">":
                                            if stringOrNumber == "string":
                                                if str(node.getAttribute(attribute)) > str(literal):
                                                    f.write(node.toxml())
                                            elif stringOrNumber == "number":
                                                if float(node.getAttribute(attribute)) > float(literal):
                                                    f.write(node.toxml())
                                        #Unknown relation operator was set
                                        else:
                                            sys.stderr.write("Bad relation operator\n" )
                                            sys.exit(80) 
                            #Specified element is not set
                            else:
                                #Giong thru all child elements
                                for child in node.childNodes: 
                                    #If we find element of the same name as in WHERE was set                       
                                    if child.nodeName == where_var:
                                        #Case of CONTAINS in condition
                                        if relation_operator == "CONTAINS":
                                            if literal in child.firstChild.nodeValue:
                                                f.write(node.toxml())
                                                counter += 1
                                        #Case of equal operator in condition
                                        elif relation_operator == "=":
                                            if child.firstChild.nodeValue == literal:
                                                f.write(node.toxml())
                                                counter += 1
                                        #Case of less operator in condition
                                        elif relation_operator == "<":                                
                                            if child.firstChild.nodeValue < literal:
                                                f.write(node.toxml())
                                                counter += 1
                                        #Case of grater operator in condition
                                        elif relation_operator == ">":
                                            if child.firstChild.nodeValue > literal:
                                                f.write(node.toxml())
                                                counter += 1
                                        #Unknown relation operator was set
                                        else:
                                            sys.stderr.write("Bad relation operator\n" )
                                            sys.exit(80)                            
                        #WHERE was not set
                        else:
                            f.write(node.toxml())
                            counter += 1
                #If LIMIT is not set
                else:
                    #If WHERE is set         
                    if where_var:                        
                        #We are selecting specified element
                        if where_var.startswith('.'):
                            attribute = ''.join(where_var.split('.')[1:])
                            if node.hasAttribute(attribute):
                                #Case of CONTAINS in condition
                                if relation_operator == "CONTAINS":
                                    if literal in node.getAttribute(attribute):
                                        f.write(node.toxml())
                                #Case of equal operator in condition
                                elif relation_operator == "=":
                                    if stringOrNumber == "string":
                                        if str(node.getAttribute(attribute)) == str(literal):
                                            f.write(node.toxml())
                                    elif stringOrNumber == "number":
                                        if float(node.getAttribute(attribute)) == float(literal):
                                            f.write(node.toxml())
                                #Case of less operator in condition
                                elif relation_operator == "<":                            
                                    if stringOrNumber == "string":
                                        if str(node.getAttribute(attribute)) < str(literal):
                                            f.write(node.toxml())
                                    elif stringOrNumber == "number":
                                        if float(node.getAttribute(attribute)) < float(literal):
                                            f.write(node.toxml())
                                #Case of grater operator in condition
                                elif relation_operator == ">":
                                    if stringOrNumber == "string":
                                        if str(node.getAttribute(attribute)) > str(literal):
                                            f.write(node.toxml())
                                    elif stringOrNumber == "number":
                                        if float(node.getAttribute(attribute)) > float(literal):
                                            f.write(node.toxml())
                                #Unknown relation operator was set
                                else:
                                    sys.stderr.write("Bad relation operator\n" )
                                    sys.exit(80) 
                        elif "." in where_var:
                            #We are selecting specified elemnt
                            if not where_var.startswith('.'):
                                #Split element name and attribute name
                                attribute = ''.join(where_var.split('.')[1:])
                                element = where_var.split(".")[0]
                                #Get all elements with specified attribute
                                if node.hasAttribute(attribute):
                                    #Data types ar not the same
                                    if stringOrNumber == "number" and not node.getAttribute(attribute).isdigit():
                                        if args.root:
                                            f.write("</"+args.root+">")            
                                        #Close file 
                                        f.close()
                                        sys.exit(0)
                                    #Case of CONTAINS in condition
                                    if relation_operator == "CONTAINS":
                                        if literal in node.getAttribute(attribute):
                                            f.write(node.toxml())
                                    #Case of equal operator in condition
                                    elif relation_operator == "=":
                                        if stringOrNumber == "string":
                                            if str(node.getAttribute(attribute)) == str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(node.getAttribute(attribute)) == float(literal):
                                                f.write(node.toxml())
                                    #Case of less operator in condition
                                    elif relation_operator == "<":                            
                                        if stringOrNumber == "string":
                                            if str(node.getAttribute(attribute)) < str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(node.getAttribute(attribute)) < float(literal):
                                                f.write(node.toxml())
                                    #Case of grater operator in condition
                                    elif relation_operator == ">":
                                        if stringOrNumber == "string":
                                            if str(node.getAttribute(attribute)) > str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(node.getAttribute(attribute)) > float(literal):
                                                f.write(node.toxml())
                                    #Unknown relation operator was set
                                    else:
                                        sys.stderr.write("Bad relation operator\n" )
                                        sys.exit(80)
                        #Specified element is not set
                        else:
                            #Giong thru all child elements
                            for child in node.childNodes: 
                                #If we find element of the same name as in WHERE was set                       
                                if child.nodeName == where_var:
                                    #Case of CONTAINS in condition
                                    if relation_operator == "CONTAINS":
                                        if literal in child.firstChild.nodeValue:
                                            f.write(node.toxml())
                                    #Case of equal operator in condition
                                    elif relation_operator == "=":
                                        if stringOrNumber == "string":                            
                                            if str(child.firstChild.nodeValue) == str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(child.firstChild.nodeValue) == float(literal):
                                                f.write(node.toxml())
                                    #Case of less operator in condition
                                    elif relation_operator == "<":
                                        if stringOrNumber == "string":                            
                                            if str(child.firstChild.nodeValue) < str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(child.firstChild.nodeValue) < float(literal):
                                                f.write(node.toxml())
                                    #Case of grater operator in condition
                                    elif relation_operator == ">":
                                        if stringOrNumber == "string":                            
                                            if str(child.firstChild.nodeValue) > str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(child.firstChild.nodeValue) > float(literal):
                                                f.write(node.toxml())
                                    #Unknown relation operator was set
                                    else:
                                        sys.stderr.write("Bad relation operator\n" )
                                        sys.exit(80)                           
                    #WHERE was not set                
                    else:
                        f.write(node.toxml())

        #We are selecting element with specified attribute
        elif "." in from_var:
            #We are selecting specified elemnt
            if not from_var.startswith('.'):
                #Split element name and attribute name
                attribute = ''.join(from_var.split('.')[1:])
                element = from_var.split(".")[0]
                #Get all elements with specified attribute
                for node in dom.getElementsByTagName(element):
                    if node.hasAttribute(attribute):
                        for node2 in node.getElementsByTagName(select_var):
                            #If LIMIT is set
                            if limit_var:
                                if counter < int(limit_var):
                                    #If WHERE is set
                                    if where_var:
                                        #Giong thru all child elements
                                        for child in node2.childNodes: 
                                            #If we find element of the same name as in WHERE was set                       
                                            if child.nodeName == where_var:
                                                #Case of CONTAINS in condition
                                                if relation_operator == "CONTAINS":
                                                    if literal in child.firstChild.nodeValue:
                                                        f.write(node2.toxml())
                                                        counter += 1
                                                #Case of equal operator in condition
                                                elif relation_operator == "=":
                                                    if stringOrNumber == "string":                            
                                                        if str(child.firstChild.nodeValue) == str(literal):
                                                            f.write(node2.toxml())
                                                            counter += 1
                                                    elif stringOrNumber == "number":
                                                        if float(child.firstChild.nodeValue) == float(literal):
                                                            f.write(node2.toxml())
                                                            counter += 1
                                                #Case of less operator in condition
                                                elif relation_operator == "<":                                
                                                    if stringOrNumber == "string":                            
                                                        if str(child.firstChild.nodeValue) < str(literal):
                                                            f.write(node2.toxml())
                                                            counter += 1
                                                    elif stringOrNumber == "number":
                                                        if float(child.firstChild.nodeValue) < float(literal):
                                                            f.write(node2.toxml())
                                                            counter += 1
                                                #Case of grater operator in condition
                                                elif relation_operator == ">":
                                                    if stringOrNumber == "string":                            
                                                        if str(child.firstChild.nodeValue) > str(literal):
                                                            f.write(node2.toxml())
                                                            counter += 1
                                                    elif stringOrNumber == "number":
                                                        if float(child.firstChild.nodeValue) > float(literal):
                                                            f.write(node2.toxml())
                                                            counter += 1
                                                #Unknown relation operator was set
                                                else:
                                                    sys.stderr.write("Bad relation operator\n" )
                                                    sys.exit(80)                            
                                    #WHERE was not set
                                    else:
                                        f.write(node2.toxml())
                                        counter += 1
                            #If LIMIT is not set
                            else:       
                                #If WHERE is set         
                                if where_var:
                                    #Going thru all child elements
                                    for child in node.childNodes:  
                                        #If we find element of the same name as in WHERE was set                        
                                        if child.nodeName == where_var:
                                            #Case of CONTAINS in condition
                                            if relation_operator == "CONTAINS":
                                                if literal in child.firstChild.nodeValue:
                                                    f.write(node2.toxml())
                                            #Case of equal operator in condition
                                            elif relation_operator == "=":
                                                if stringOrNumber == "string":                            
                                                    if str(child.firstChild.nodeValue) == str(literal):
                                                        f.write(node2.toxml())
                                                elif stringOrNumber == "number":
                                                    if float(child.firstChild.nodeValue) == float(literal):
                                                        f.write(node2.toxml())
                                            #Case of less operator in condition
                                            elif relation_operator == "<":                                
                                                if stringOrNumber == "string":                            
                                                    if str(child.firstChild.nodeValue) < str(literal):
                                                        f.write(node2.toxml())
                                                elif stringOrNumber == "number":
                                                    if float(child.firstChild.nodeValue) < float(literal):
                                                        f.write(node2.toxml())
                                            #Case of grater operator in condition
                                            elif relation_operator == ">":
                                                if stringOrNumber == "string":                            
                                                    if str(child.firstChild.nodeValue) > str(literal):
                                                        f.write(node2.toxml())
                                                elif stringOrNumber == "number":
                                                    if float(child.firstChild.nodeValue) > float(literal):
                                                        f.write(node2.toxml())
                                            #Unknown relation operator was set
                                            else:
                                                sys.stderr.write("Bad relation operator\n" )
                                                sys.exit(80)               
                                #WHERE was not set                
                                else:
                                    f.write(node2.toxml())                                   
            #We are selecting only specified attribute - No limit can be used
            else:
                attribute = from_var[1:]            
                for node in dom.getElementsByTagName(query_args[1]):
                    parent = node.parentNode
                    if parent.hasAttribute(attribute):
                        f.write(node.toxml())
                        break
        #We are selecting from specified element
        else:
            element = dom.getElementsByTagName(from_var)[0]
            for node in element.getElementsByTagName(select_var):
                #If LIMIT is set
                if limit_var:
                    if counter < int(limit_var):
                        #If WHERE is set
                        if where_var:
                            #Giong thru all child elements
                            for child in node.childNodes: 
                                #If we find element of the same name as in WHERE was set                       
                                if child.nodeName == where_var:
                                    #Case of CONTAINS in condition
                                    if relation_operator == "CONTAINS":
                                        if literal in child.firstChild.nodeValue:
                                            f.write(node.toxml())
                                            counter += 1
                                    #Case of equal operator in condition
                                    elif relation_operator == "=":
                                        if stringOrNumber == "string":                            
                                            if str(child.firstChild.nodeValue) == str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(child.firstChild.nodeValue) == float(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                    #Case of less operator in condition
                                    elif relation_operator == "<":
                                        if stringOrNumber == "string":                            
                                            if str(child.firstChild.nodeValue) < str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(child.firstChild.nodeValue) < float(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                    #Case of grater operator in condition
                                    elif relation_operator == ">":
                                        if stringOrNumber == "string":                            
                                            if str(child.firstChild.nodeValue) > str(literal):
                                                f.write(node.toxml())
                                        elif stringOrNumber == "number":
                                            if float(child.firstChild.nodeValue) > float(literal):
                                                f.write(node.toxml())
                                                counter += 1
                                    #Unknown relation operator was set
                                    else:
                                        sys.stderr.write("Bad relation operator\n" )
                                        sys.exit(80)                           
                        #WHERE was not set
                        else:
                            f.write(node.toxml())
                            counter += 1
                #If LIMIT is not set
                else:       
                    #If WHERE is set         
                    if where_var:
                        #Going thru all child elements
                        for child in node.childNodes:  
                            #If we find element of the same name as in WHERE was set                        
                            if child.nodeName == where_var:
                                #Case of CONTAINS in condition
                                if relation_operator == "CONTAINS":
                                    if literal in child.firstChild.nodeValue:
                                        f.write(node.toxml())
                                #Case of equal operator in condition
                                elif relation_operator == "=":
                                    if stringOrNumber == "string":                            
                                        if str(child.firstChild.nodeValue) == str(literal):
                                            f.write(node.toxml())
                                    elif stringOrNumber == "number":
                                        if float(child.firstChild.nodeValue) == float(literal):
                                            f.write(node.toxml())
                                #Case of less operator in condition
                                elif relation_operator == "<":
                                    if stringOrNumber == "string":                            
                                        if str(child.firstChild.nodeValue) < str(literal):
                                            f.write(node.toxml())
                                    elif stringOrNumber == "number":
                                        if float(child.firstChild.nodeValue) < float(literal):
                                            f.write(node.toxml())
                                #Case of grater operator in condition
                                elif relation_operator == ">":
                                    if stringOrNumber == "string":                            
                                        if str(child.firstChild.nodeValue) > str(literal):
                                            f.write(node.toxml())
                                    elif stringOrNumber == "number":
                                        if float(child.firstChild.nodeValue) > float(literal):
                                            f.write(node.toxml())
                                #Unknown relation operator was set
                                else:
                                    sys.stderr.write("Bad relation operator\n" )
                                    sys.exit(80)                      
                    #WHERE was not set                
                    else:
                        f.write(node.toxml())

        #If specific root element is specified - end element
        if args.root:
            f.write("</"+args.root+">")
            
        #Close file 
        f.close()
   
#Catching common errors 
except IndexError:
    sys.stderr.write("xxxSemantic error\n" )
    sys.exit(80)

except ValueError:
    sys.stderr.write("Value Error\n" )
    sys.exit(100)

except OSError:
    sys.stderr.write("File Error\n" )
    sys.exit(2)

except IOError:
    sys.stderr.write("File Error\n" )
    sys.exit(3)

#No error
sys.exit(0)
