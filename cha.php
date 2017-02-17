<?php 
#UTF-8
#CHA: xfitzj00
#AUTHOR: Jan Fitz
#DATE: 2016

//Set correct UTF-8 encoding
iconv_set_encoding("internal_encoding", "UTF-8");
//Turn off the warnings
error_reporting(E_ALL ^ E_WARNING); 

//Define error codes
define("NO_ERROR", 0);
define("PARAMS_ERR", 1);
define("ERROR_IO", 2);
define("FILE_ERR", 3);

//Define flags
$isDir = false;
$isFile = false;
$outputSet = false;
$prettySet = false;
$noinlineSet = false;
$removeWhitespace = false;
$noDuplicates = false;
$maxParams = 0;
$maxParamsSet = false;
$content = '';
$xmlIterator = 0;
$header = '<?xml version="1.0" encoding="UTF-8"?>';
$result = '';
$result .= $header; 

//Function for printing help
function printHelp()
{
	echo "\t----------------------------------------------------------------------------------\n";
	echo "\t| Skript pro analyzu hlavickovych souboru jazyka C podle standartu ISO C99,       |\n";
	echo "\t| ktery vytvori databazi nalezenych funkci v techto souborech ve formatu xml      |\n";
	echo "\t|                                                                                 |\n";
	echo "\t| Pripustne paramatry skriptu:                                                    |\n";
	echo "\t|\t  --help                                                                  |\n";
	echo "\t|\t  --input=fileordir                                                       |\n";
	echo "\t|\t  --output=filename                                                       |\n";
	echo "\t|\t  --pretty-xml=k                                                          |\n";
	echo "\t|\t  --no-inline                                                             |\n";
	echo "\t|\t  --max-par=n                                                             |\n";
	echo "\t|\t  --no-duplicates                                                         |\n";
	echo "\t|\t  --remove-whitespace                                                     |\n";
	echo "\t|                                                                                 |\n";
	echo "\t| Vsechny parametry jsou nepovinne a na jejich poradi nezalezi. Soubory i adresare|\n";
	echo "\t| mohou byt zadany relativni i absolutni cestou                                   |\n";
	echo "\t-----------------------------------------------------------------------------------\n";
}

//Look for all header files in dir
function scannig_dir(&$arr, $path, $content, $indent, $outputFile) 
{
	$iter = new RecursiveDirectoryIterator($path);
	foreach(new RecursiveIteratorIterator($iter) as $filename => $file) 

	{
		$result = '';
		if (preg_match("/\.h$/", $filename))
		 {
			$arr[] = $filename;
			$content = file_get_contents($filename, FILE_SKIP_EMPTY_LINES);

			//Remove everything which is not needet from files
			$content = remove_the_rest($content);
			
			//Calling function for writing content into xml file
			xml_write($outputFile, get_functions($content, $filename), $indent);			
		}
	}	
	return $content;
}

//Remove everythig bad from input files
function remove_the_rest($content) 
{
	//Remove slash coments from file
	$content = preg_replace('![ \t]*//.*[ \t]*[\r\n]!', '', $content);

	//Remove one line coments with stars from file
	$content = preg_replace('!/\*.*?\*/!s', '', $content);

	//Remove  multipleline comments with stars from file
	$content = preg_replace('/\n\s*\n/', "\n", $content);
	
	//Remove macros from file
	$content = preg_replace('!#.*!', '', $content);

	//Remove content inside curly paretnheses
	$content = preg_replace('!.*{([^}]*)}.*!', '', $content);

	//Remove blank lines
	$content = preg_replace('/^[ \t]*[\r\n]+/m', '', $content);

	return $content;
}

//Get all functions in file to write into xml
function get_functions($content, $filename) 
{		
	$content = str_replace("\n", ' ', $content);
	$content = str_replace(";", ";\n", $content);
	$content = str_replace(") ;", ");", $content);
	$content = str_replace(") ", ")\n", $content);
	
	$tempContent = $content;
	$numberOfLines = substr_count($tempContent, "\n" );	
	
	//Get all function types
	for($i = 0; $i < $numberOfLines; $i++)
	{
		$filenames[$i] = $filename;
		if(preg_match('/\s*(?<retType>(?:\s*[A-Za-z_]\w*[\s\*]+)+)\s*(?<funcName>(?:[A-Za-z_]\w*))\s*\((?<params>(?:[\s\S]*?)*)\)\s*[;|{]/', $tempContent, $temp))
		{
			$type[$i] = trim($temp[1]);		
			$tempContent = preg_replace('/^.+\n/', '', $tempContent);
		}			
	}
	
	//If nothing to parse, return without erro
	if(empty($type))
	{
		return 0;
	}  

	//Put data into result array
	$functions[0] = $filenames;
	$functions[1] = $type;
	$tempContent = $content;

	//Get all function names	
	for($i = 0; $i < $numberOfLines; $i++)
	{
		if(preg_match('/\s*(?<retType>(?:\s*[A-Za-z_]\w*[\s\*]+)+)\s*(?<funcName>(?:[A-Za-z_]\w*))\s*\((?<params>(?:[\s\S]*?)*)\)\s*[;|{]/', $tempContent, $temp))
		{
			$temp = str_replace(' ', '', $temp); //Delete unnecessary spaces form name
			$name[$i] = trim($temp[2]);
			$tempContent = preg_replace('/^.+\n/', '', $tempContent);
		} 		
	}   

	$functions[2] = $name;
	$tempContent = $content;

	//Get params of function
	for($i = 0; $i < $numberOfLines; $i++)
	{	
		$zero_params = preg_match('/\((.*?)\)/', $tempContent, $temp);	

		if($zero_params)
		{
			if($temp[0] == "()")
			{
				$zero_params = 0;
			}	
			else
			{
				$zero_params = 1;
			}
		}
		
		//Case when function has zero parametrs
		if($zero_params != 0)
		{ 
			$params[$i] = trim($temp[1]);
			$tempContent = preg_replace('/^.+\n/', '', $tempContent);
			$numberOfParams[$i] = substr_count($params[$i], ",") + 1;
		}
		else
		{
			$params[$i] = "";
			$tempContent = preg_replace('/^.+\n/', '', $tempContent);
			$numberOfParams[$i] = 0;
		}

		//Check variable amount of vars
		if (strpos($params[$i], '...'))
		{
    		$varargs[$i] = "yes";
    		$numberOfParams[$i] -= 1;    		
    		$params[$i] = str_replace( '...', '', $params[$i]);
    		//Delete last character from parametr with variable amount of params - it will be comma
    		$params[$i] = substr($params[$i], 0, -2);
		}
		else
		{
			$varargs[$i] = "no";
		}		
	}

	$functions[3] = $numberOfParams;
	$functions[4] = $params;
	$functions[5] = $varargs;

	//Delete function with more params that is allowed
	if($GLOBALS["maxParamsSet"] === true)
	{
		for($i = 0; $i < count($functions[3]); $i++)
		{
			if($functions[3][$i] > $GLOBALS["maxParams"])
			{
				array_splice($functions[0], $i, 1);
				array_splice($functions[1], $i, 1);
				array_splice($functions[2], $i, 1);
				array_splice($functions[3], $i, 1);
				array_splice($functions[4], $i, 1);
				array_splice($functions[5], $i, 1);
			}
		}
	}

	//Delete function define like inline
	if($GLOBALS["noinlineSet"] === true)
	{
		for($i = 0; $i < count($functions[1]); $i++)
		{
			if(preg_match('/inline/', $functions[1][$i]))
			{				
				array_splice($functions[0], $i, 1);
				array_splice($functions[1], $i, 1);
				array_splice($functions[2], $i, 1);
				array_splice($functions[3], $i, 1);
				array_splice($functions[4], $i, 1);
				array_splice($functions[5], $i, 1);
				$i = $i - 1;
			}
		}
	}

	//Delete duplicates
	if($GLOBALS["noDuplicates"] === true)
	{
		$count = count($functions[2]);
		for($i = 0; $i < $count; $i++)
		{
			for($j = 0; $j < $count-1; $j++)
			{
				if($i == $j+1)
				{

				}
				else if((((string)$functions[2][$i]) == ((string)$functions[2][$j+1])) && ($i > $j+1))
				{					
					array_splice($functions[0], $j+1, 1);
					array_splice($functions[1], $j+1, 1);
					array_splice($functions[2], $j+1, 1);
					array_splice($functions[3], $j+1, 1);
					array_splice($functions[4], $j+1, 1);
					array_splice($functions[5], $j+1, 1);																
				}
			}
		}
	}

	//Remove whitespaces
	if($GLOBALS["removeWhitespace"] === true)
	{
		for($i = 0; $i < count($functions[1]); $i++)
		{
			(string)$functions[0][$i] = preg_replace('/\s\s+/', ' ', (string)$functions[0][$i]);
			(string)$functions[1][$i] = preg_replace('/\s\s+/', ' ', (string)$functions[1][$i]);
			(string)$functions[2][$i] = preg_replace('/\s\s+/', ' ', (string)$functions[2][$i]);
			(string)$functions[3][$i] = preg_replace('/\s\s+/', ' ', (string)$functions[3][$i]);
			(string)$functions[4][$i] = preg_replace('/\s\s+/', ' ', (string)$functions[4][$i]);
			(string)$functions[5][$i] = preg_replace('/\s\s+/', ' ', (string)$functions[5][$i]);
		}		
	}

	//print_r($functions);

	return $functions;		
}

//Write into xml file
function xml_write($output, $functions, $indent) 
{
	//Count number of iterations for xml_write function
	$xmlIterator = count($functions[0]);

	//Variable for setting indent
	$indentChars = "";

	if($GLOBALS["prettySet"] === false)
	{
		$indent = 0;
		$newLine = "";
	}
	else
	{
		$GLOBALS["result"] .= "\n";
		$newLine = "\n";
	}

	//Set indent
	for($i = 0; $i < $indent; $i++)
	{
		$indentChars .= " ";
	}

	//Get name of dir
	//When wasn't set input file or directory
	if ($GLOBALS["isDir"]  === false && $GLOBALS["isFile"] === false)
	{
		$dir_name = "./";
	}
	else if(preg_match("/.*\//", (string)$functions[0][0], $dir) && $GLOBALS["isDir"]  == true)
	{
		$dir_name = $dir[0];
	}
	else
	{
		$dir_name = ""; //If file is set not dir
	}
	
	$GLOBALS["result"] .= '<functions dir="'.$dir_name.'">'.$newLine;

		for($j = 0; $j < $xmlIterator; $j++)
		{
			$GLOBALS["result"] .= $indentChars.'<function file="'.(string)$functions[0][$j].'"'.' name="'.(string)$functions[2][$j].'" varargs="'.(string)$functions[5][$j].'" rettype="'.(string)$functions[1][$j].'">'.$indentChars;
		
			//Cycle for writing params into xml file
				if((int)$functions[3][$j] > 1)
				{
					$data = preg_split('/,/', (string)$functions[4][$j]); //Cutting input string by commas into array 
																	 								
					for($k = 0; $k < count($data); $k++)
					{
						for($m = 0; $m < count($data[$k]); $m++)
						{								
							preg_match('/.*(\s|\*|\*\s)/',trim((string)$data[$k]), $a); //Regex for cutting more than one param
							$GLOBALS["result"] .= $newLine.$indentChars.$indentChars.'<param number="'.($k+1).'" type="'.trim((string)$a[0]).'" />';
						}

					}
					$GLOBALS["result"] .= $newLine;								
				}
				else if((int)$functions[3][$j] == 0) //When no paramter is set
				{				
					$GLOBALS["result"] .= $newLine;
				}
				else if((int)$functions[3][$j] == 1) //When only one parametr is set
				{
					//Function has parametres set as void
					if((string)$functions[4][$j] == "void")
					{
						$GLOBALS["result"] .= $newLine;
					}
					else
					{
						preg_match('/.*(\s|\*|\*\s)/',trim((string)$functions[4][$j]), $a); //Regex for cutting param to *

						$GLOBALS["result"] .= $newLine.$indentChars.$indentChars.'<param number="1" type="'.(string)$a[0].'" />'.$newLine;						
					}					
				} 
			$GLOBALS["result"] .= $indentChars.'</function>'.$newLine; 
		}

		$GLOBALS["result"] .= '</functions>';
	
	//Test if output will go to file or into terminal
	if($GLOBALS["outputSet"] === true)
	{			
		file_put_contents($output, $GLOBALS["result"]); //Write data into file
	}
	//Print result into standart output
	else
	{
		print($GLOBALS["result"]);
	}

	return $GLOBALS["result"];
}

//All script params which can be used
$longops1 = array("help::", "input:", "output:", "max-par:", "pretty-xml::",	"no-inline::", "no-duplicates::", "remove-whitespace::");

//Load params from command line
$params = getopt(NULL, $longops1);

if ($params === false) 
{
	fwrite(STDERR, "Something is wrong with command line arguments! GETOPT function error\n");
	exit(ERR_PARAMS);
}

//Check correct parametrs
$longops2 = array("--help", "--input", "--output", "--max-par", "--pretty-xml",	"--no-inline", "--no-duplicates", "--remove-whitespace");

for($i = 0; $i < count($argv); $i++)
{	
	$found = false;
	if(preg_match('/(\-\-\w*\-\w*)|(\-\-\w*)/', $argv[$i], $params_field))
	{		
		for($j = 0; $j < count($longops2); $j++)
		{
			if($params_field[0] == $longops2[$j])
			{
				$found = true;
			}
		}
		if($found == false)
		{
			fwrite(STDERR, "Bad param!\n");
			exit(PARAMS_ERR);
		}	
	}	
}

//Processing command line params
foreach(array_keys($params) as $selected) {
	switch ($selected) {
		case "help":
			if(count($params) === 1) 
			{
				printHelp(); //Help will be printed out
			} 
			else 
			{
				fwrite(STDERR, "\t If param --help is set, nothing else can be added as command line param!\n");
				exit(PARAMS_ERR);
			}	
			exit(NO_ERROR);						
			break;

		case "input":
			if(is_dir($params[$selected]))
			{				
				$GLOBALS["isDir"] = true;
				$dirForScanning = $params[$selected];
											
			}
			else if(!is_dir($params[$selected]))
			{
				$GLOBALS["isFile"] = true;
				//Get content of input file
				$content = file_get_contents($params[$selected], true);
				if(!$content)
				{
					fwrite(STDERR, "IO error\n");
					exit(ERROR_IO);
				}
				
				$filename = $params[$selected];
			}			
			break;

		case "output":
			$outputSet = true;
			$outputFile = $params[$selected];
			$output = fopen($params[$selected], 'w');
			
			//Check correct opening of file
			if (!$output) 
			{
				fwrite(STDERR, "It is imposible to open output file!\n");
				exit(FILE_ERR);
			}						
			break;

		case "max-par":
			$GLOBALS["maxParams"] = $params[$selected];
			$GLOBALS["maxParamsSet"] = true;
			if(!is_numeric($GLOBALS["maxParams"]))
			{
				fwrite(STDERR, "Argument --max-params have to be set only as number!\n");
				exit(PARAMS_ERR);
			}
			break;

		case "pretty-xml":
			$prettySet = true;
			$GLOBALS["indent"] = $params[$selected];			
			break;

		case "no-inline":
			$GLOBALS["noinlineSet"] = true;				
			break;

		case "no-duplicates":
			$GLOBALS["noDuplicates"] = true;			
			break;

		case "remove-whitespace":
			$removeWhitespace = true;			
			break;
		
		default:
			fwrite(STDERR, "Bad params!\n");
			exit(PARAMS_ERR);
	}	
}

if($prettySet != true)
{
	$GLOBALS["indent"] = 4;
}

//We are looking for file
if($GLOBALS["isFile"] === true)
{
	//Remove everything which is not needet from files
	$content = remove_the_rest($content);
	//Calling function for writing content into xml file
	xml_write($outputFile, get_functions($content, $filename), $GLOBALS["indent"]);
}
//We are looking for directory
else if($GLOBALS["isDir"]  === true)
{	
	$content = scannig_dir($input, $dirForScanning, $content, $GLOBALS["indent"], $outputFile);
}
//When wasn't set input file or directory
else if ($GLOBALS["isDir"]  === false && $GLOBALS["isFile"] === false) {
	$content = scannig_dir($input, "./", $content, $indent, $outputFile);
}
exit(NO_ERROR);
?>
