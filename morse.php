<?php
/*
  * Description: php class for encoding and decoding strings into morse alphabet with cyech aid world
  * Author: Jan Fitz
  * Date: 2017-05-16
  * Version: v1.0
  * usage: php morse.php Hi/I/am/Morse || php morse.php ..../..//..//.-/--//--/---/.-./..././
*/

class Morse {

	/**
	* Encode morse string: text -> morse
	*
	* @param string morseString  	input string
	*
	* @return boolean				      exit status
	*
	*/
	public function morseEncode($morseString) {
			echo 'Encoding string: '. $morseString . "\n";
      $morseArray = str_split($morseString);

			echo 'Encoded string: ';
      foreach ($morseArray as $char) {
				if($char != "/") {
					$char = strtoupper($char);
	        echo $this->morseCompare($char, "encode"). "/";
				}
				else {
					echo "/";
				}
      }
    	return 0;
  }

  /**
	* Decode morse string: morse -> text
	*
	* @param string morseString  	input morse string
	*
	* @return boolean 			      exit status
	*
	*/
	public function morseDecode($morseString) {
			$morseLength = strlen($morseString);
		  $morseArray = str_split($morseString);
      $morseConcat = "";
			$counter = 0;
			$echoSpace = false;

			echo "\n". 'Decoded string: ';

      foreach ($morseArray as $char) {
        $morseConcat .= $char;
				if($echoSpace == true) {
					echo " ";
					$echoSpace = false;
				}
        if($char == "/") {
					if($counter < $morseLength - 1) {
						if($morseArray[$counter+1] == "/") {
							$echoSpace = true;
						}
					}
          $morseString = substr($morseConcat, 0, -1);
          echo $this->morseCompare($morseString, "decode");
          $morseConcat = "";
        }
				$counter++;
      }
			echo "\n";
    	return true;
  }

	/**
	* Get key or value from morse alphaber array
	*
	* @param string morseChar  		input morse char
	* @param string	op						type of operation: encode/decode
	*
	* @return string 				      decode/encode string
	*
	*/
  public function morseCompare($morseChar, $op) {
    
    $alphabet = array(
       "A" => ".-", // Akát
       "B" => "-...", // Blýskavice
       "C" => "-.-.", // Cílovníci
       "D" => "-..", // Dálava
       "E" => ".", // Erb
       "F" => "..-.", // Filipíny
       "G" => "--.", // Grónská zem
       "H" => "....", // Hrachovina
       "CH" => "----", // Chléb nám dává
       "I" => "..", // Ibis
       "J" => ".---", // Jasmín bílý
       "K" => "-.-", // Krákora
       "L" => ".-..", // Lupíneček
       "M" => "--", // Mává
       "N" => "-.", // Nástup
       "O" => "---", // Ó náš pán
       "P" => ".--.", // Papírníci
       "Q" => "--.-", // Kvílí orkán
       "R" => ".-.", // Rarášek
       "S" => "...", // Sobota
       "T" => "-", // Tón
       "U" => "..-", // Učený
       "V" => "...-", // Vyvolený
       "W" => ".--", // Waltrův vůz
       "X" => "-..-", // Xénie má
       "Y" => "-.--", // ý se ztrácí
       "Z" => "--.." // známá žena
     );

    if ($op == "encode") {
      return $alphabet[$morseChar];
    }
    elseif ($op == "decode") {
      return array_search($morseChar, $alphabet);
    }
  }
}

$obj = new Morse; // New instance of object

/* Recognize string to be encode or decode */
if ($argv[1][0] == "." || $argv[1][0] == "-" || $argv[1][0] == "/") {
  echo 'Decoding string: ' . str_replace(' ', '', $argv[1]);
  $obj->morseDecode(str_replace(' ', '', $argv[1]));
}
else {
  $obj->morseEncode($argv[1]);
}
?>
