<?php
  // Specify SQL query and credentials
  $sqlQuery = "";
  $databaseHost = '';
  $databaseUser = '';
  $databasePassword = '';

  // Init databasesNames Array
  $databasesNames = array();
  $counter = 0;

  // Array of databases configuration: host, user, password, database
  $informationSchema = array('host', 'user', 'passwd', 'information_schema');

  // Connect to information_schema database
  $db_spojeni = mysqli_connect (
    $informationSchema[0],
    $informationSchema[1],
    $informationSchema[2],
    $informationSchema[3]
  );

  // SQL query on databases names
  $objekt_vysledku = mysqli_query($db_spojeni, "SELECT * FROM schemata");

  // Fill array of databases names
  while ($row = mysqlI_fetch_array($objekt_vysledku))
  {
    $result = $row['SCHEMA_NAME'];
    $databasesNames[$counter] = $result;
    $counter++;
  }

  // Execute SQL query over all available databases
  foreach ($databasesNames as $databaseName) {
    // Connect to database
    $db_spojeni = mysqli_connect (
      $databaseHost,
      $databaseUser,
      $databasePassword,
      $databaseName
    );

    $objekt_vysledku = mysqli_query($db_spojeni, $sqlQuery);

    if($objekt_vysledku) {
      // Print query info
      echo "<br />Following query over <b>". $databaseName ."</b> database terminated <font color='green'>successful</font><br />";
      echo "<pre>". $sqlQuery ."</pre>";

      // Exucute result
      while ($row = mysqlI_fetch_array($objekt_vysledku))
      {
        $result = $row['name'];
        echo "<b>Result: </b>". $result;
        echo "<br /><hr />";
      }
    }
    else {
      echo "<br />Query over <b>". $databaseName ."</b> database <font color='red'>can't be reached</font><br /><hr />";
    }
  }
?>
