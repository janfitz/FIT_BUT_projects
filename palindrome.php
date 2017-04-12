<?php
/*
  * PHP class for solving palindrome problem. Given json
  * file or input string is compared with reversed one twin
  * Author: Jan Fitz
  * Date: 2017-04-12
  * Version: v1.0
*/
  class Palindrome {
    /*
      * Function with process given input file or input string
    */
    public function start($string) {
      // Process from json file
      if(file_exists($string)) {
        if(($json = file_get_contents('palindrome.json'))) {
          if(!($json = json_decode($json))) {
            echo "Unrecognized json syntax";
          }
        }
        else {
          echo "Unable to one input json file";
        }

        // Loop over all test strings in json file
        while ($string = current($json)) {
          $string = $this->compare($this->normalize($string));
          next($json);
        }
      }
      // Process from given string
      else {
        $string = $this->compare($string);
      }
    }

    /*
      * Function for normalizing given string
      * string to lower case, skip diacritic
    */
    public function normalize($string) {
      // Normalize string - make string lowercase, ...
      $string = preg_replace('/[^a-zA-Z0-9]/', '', iconv('UTF-8', 'ASCII//TRANSLIT', str_replace(' ', '', strtolower($string->string))));
      return $string;
    }

    /*
      * Function for comparing reversed input string with input string
    */
    public function compare($string) {
      // Compare input string with reversed input string
      if(strrev($string) === $string) {
        echo $string . ": IT IS palindrome \n";
      }
      else {
        echo $string . ": IT IS NOT palindrome \n";
      }
      return $string;
    }
  }
  // New instance of palindrome class
  $palindrome = new Palindrome;

  // Process teminal arguments
  if(!empty($argv[1])) {
    $palindrome->start($argv[1]);
  }
  else {
    echo "Missing parametr";
  }
?>
