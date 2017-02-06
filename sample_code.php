// Sample php code as subfile of web project based on PHP a MySQL
// Author: Jan Fitz
// Date: July 2016

$nazev = htmlspecialchars($_POST['nazev']);
$popis = ($_POST['popis']);
$zamereni = htmlspecialchars($_POST['zamereni']);
$web = htmlspecialchars($_POST['web']);
$email = htmlspecialchars($_POST['email']);
$prvni_clen = htmlspecialchars($_POST['prvni_clen']);
$druhy_clen = htmlspecialchars($_POST['druhy_clen']);
$treti_clen = htmlspecialchars($_POST['treti_clen']);
$ctvrty_clen = htmlspecialchars($_POST['ctvrty_clen']);
$paty_clen = htmlspecialchars($_POST['paty_clen']);
$sesty_clen = htmlspecialchars($_POST['sesty_clen']);
$facebook = htmlspecialchars($_POST['facebook']);
$twitter = htmlspecialchars($_POST['twitter']);
$gplus = htmlspecialchars($_POST['gplus']);
$jina_sit = htmlspecialchars($_POST['jina_sit']);
$sidlo = htmlspecialchars($_POST['sidlo']);
$poznamky = htmlspecialchars($_POST['poznamky']);
$keywords = htmlspecialchars($_POST['keywords']);
$antispam = htmlspecialchars($_POST['antispam']);

$popis = nl2br($popis);

$schvaleno = "ne";

$pattern = '/^(?!(?:(?:\\x22?\\x5C[\\x00-\\x7E]\\x22?)|(?:\\x22?[^\\x5C\\x22]\\x22?)){255,})(?!(?:(?:\\x22?\\x5C[\\x00-\\x7E]\\x22?)|(?:\\x22?[^\\x5C\\x22]\\x22?)){65,}@)(?:(?:[\\x21\\x23-\\x27\\x2A\\x2B\\x2D\\x2F-\\x39\\x3D\\x3F\\x5E-\\x7E]+)|(?:\\x22(?:[\\x01-\\x08\\x0B\\x0C\\x0E-\\x1F\\x21\\x23-\\x5B\\x5D-\\x7F]|(?:\\x5C[\\x00-\\x7F]))*\\x22))(?:\\.(?:(?:[\\x21\\x23-\\x27\\x2A\\x2B\\x2D\\x2F-\\x39\\x3D\\x3F\\x5E-\\x7E]+)|(?:\\x22(?:[\\x01-\\x08\\x0B\\x0C\\x0E-\\x1F\\x21\\x23-\\x5B\\x5D-\\x7F]|(?:\\x5C[\\x00-\\x7F]))*\\x22)))*@(?:(?:(?!.*[^.]{64,})(?:(?:(?:xn--)?[a-z0-9]+(?:-+[a-z0-9]+)*\\.){1,126}){1,}(?:(?:[a-z][a-z0-9]*)|(?:(?:xn--)[a-z0-9]+))(?:-+[a-z0-9]+)*)|(?:\\[(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){7})|(?:(?!(?:.*[a-f0-9][:\\]]){7,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?)))|(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){5}:)|(?:(?!(?:.*[a-f0-9]:){5,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3}:)?)))?(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))(?:\\.(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))){3}))\\]))$/iD';

$ip = $_SERVER["REMOTE_ADDR"];

utf8_encode($nazev);
utf8_encode($popis);
utf8_encode($zamereni);
utf8_encode($web);
utf8_encode($email);
utf8_encode($prvni_clen);
utf8_encode($druhy_clen);
utf8_encode($treti_clen);
utf8_encode($ctvrty_clen);
utf8_encode($paty_clen);
utf8_encode($sesty_clen);
utf8_encode($facebook);
utf8_encode($twitter);
utf8_encode($gplus);
utf8_encode($jina_sit);
utf8_encode($sidlo);
utf8_encode($keywords);

if (empty($nazev)) {
  $missing_nazev = '<input type="text" name="nazev" placeholder="Název projektu" style="background-color: #ef005c;" required>';
  $empty_nazev = 0;
}
else {
  $missing_nazev = '<input type="text" name="nazev" placeholder="Název projektu" required>';
  $empty_nazev = 1;
}

if (empty($popis)) {
  $missing_popis = '<textarea name="popis" placeholder="Popis Projektu" style="width: 400px; height: 250px; background-color: #ef005c;" required></textarea>';
  $empty_popis = 0;
}
else {
  $missing_popis = '<textarea name="popis" placeholder="Popis Projektu" style="width: 400px; height: 250px;" required></textarea>';
  $empty_popis = 1;
}

if (empty($web)) {
  $missing_web = '<input type="text" name="web" placeholder="Web" style="background-color: #ef005c;" required>';
  $empty_web = 0;
}
else {
  $missing_web = '<input type="text" name="web" placeholder="Web" required>';
  $empty_web = 1;
}

if (empty($email)) {
  $missing_email = '<input type="text" name="email" placeholder="Email" style="background-color: #ef005c;" required>';
  $empty_email = 0;
}
else {
  $missing_email = '<input type="text" name="email" placeholder="Email" required>';
  $empty_email = 1;
}

if (empty($prvni_clen)) {
  $missing_prvni_clen = '<input type="text" name="prvni_clen" placeholder="První člen teamu" style="background-color: #ef005c;" required>';
  $empty_prvni_clen = 0;
}
else {
  $missing_prvni_clen = '<input type="text" name="prvni_clen" placeholder="První člen teamu" required>';
  $empty_prvni_clen = 1;
}

if (empty($zamereni)) {
  $missing_zamereni = '<select name="zamereni" style="background-color: #ef005c;">';
  $empty_zamereni = 0;
}
else {
  $missing_zamereni = '<select name="zamereni">';
  $empty_zamereni = 1;
}

if (empty($keywords)) {
  $missing_keywords = '<textarea name="keywords" placeholder="Klíčová slova" style="width: 400px; height: 120px; background-color: #ef005c;" required></textarea>';
  $empty_keywords = 0;
}
else {
  $missing_keywords = '<textarea name="keywords" placeholder="Klíčová slova" style="width: 400px; height: 120px;"" required></textarea>';
  $empty_keywords = 1;
}

if ((empty($antispam)) || ($antispam != "kmxsy")) {
  $missing_antispam = '<input type="text" name="antispam" placeholder="Opište text z obrázku" style="height: 42px; margin: 0px 0px 0px 10px; width: 240px; background-color: #ef005c;" required>';
  $empty_antispam = 0;
}
else {
  $missing_antispam = '<input type="text" name="antispam" placeholder="Opište text z obrázku" style="height: 42px; margin: 0px 0px 0px 10px; width: 240px;" required>';
  $empty_antispam = 1;
}

if ($empty_nazev == 1) {
  if ($empty_popis == 1) {
    if ($empty_zamereni == 1) {
      if ($empty_web == 1) {
        if ($empty_email == 1) {
          if ($empty_prvni_clen == 1) {
            if ($empty_keywords == 1) {
              if ($empty_antispam == 1) {
                if (preg_match($pattern, $email) === 1) {

                  include "include/log_db.php";

                  //Zjištění posledního id pro pojmenování loga projektu
                  $objekt_vysledku = mysqli_query($db_spojeni, 'SELECT * FROM projekty ORDER BY id DESC LIMIT 1');

                  while ($radek = mysqlI_fetch_array($objekt_vysledku))
                    {
                      $jmeno = $radek['id'];
                      $jmeno = $jmeno + 1;
                    }

                  $uploadDir = 'img/projects_imgs'; // adresar, kam se maji nahrat obrazky (bez lomitka na konci)
                  $allowedExt = array('jpg', 'jpeg', 'png'); // pole s povolenymi priponami

                  // zpracovani uploadu
                  if(isset($_FILES['obrazky']) && is_array($_FILES['obrazky']['name'])) {

                      $counter = 0;
                      $allowedExt = array_flip($allowedExt);
                      foreach($_FILES['obrazky']['name'] as $klic => $jmenox) {

                          $fileName = basename($jmenox);
                          $tmpName = $_FILES['obrazky']['tmp_name'][$klic];

                          // kontrola souboru
                          if(
                              !is_uploaded_file($tmpName)
                              || !isset($allowedExt[strtolower(pathinfo($fileName, PATHINFO_EXTENSION))])
                          ) {
                              // neplatny soubor nebo pripona
                              continue;
                          }

                          // presun souboru
                          if(move_uploaded_file($tmpName, "{$uploadDir}".DIRECTORY_SEPARATOR."{$jmeno}.jpg")) {
                              ++$counter;
                          }
                      }
                  }

                  $sql_prikaz =
                  "INSERT INTO projekty(nazev,popis,zamereni,poznamky,web,email,prvni_clen,druhy_clen,treti_clen,ctvrty_clen,paty_clen,sesty_clen,facebook,twitter,gplus,jina_sit,sidlo,keywords,schvaleno,ip)
                  VALUES('$nazev','$popis','$zamereni','$poznamky','$web','$email','$prvni_clen','$druhy_clen','$treti_clen','$ctvrty_clen','$paty_clen','$sesty_clen','$facebook','$twitter','$gplus','$jina_sit','$sidlo','$keywords','$schvaleno','$ip')";

                  $objekt_vysledku = mysqli_query($db_spojeni, $sql_prikaz);
                }
              }
            }
          }
        }
      }
    }
  }
}

if ($objekt_vysledku) {
  $sucessful = 'Projekt byl úspěšnem odeslán ke schválení';

  //Odeslaní emailu o novém projektu ke schválení
  $header = ("Content-Type: text/html;charset=UTF-8");
  $email = '@';
  mail($email, 'Novy projekt ke schvaleni',
  "\nByl odeslán nový projekt ke schválení, jeho správu je možné provádět v admin sekci webu.\n\n Nazev projektu: $nazev \nPoznámky: $poznamky", $header);

}
else {
  $sucessful = 'Projekt se nepodařilo odeslat ke schválení, zkuste to prosím později, nebo nás kontaktuje<br>Zkontrolujte i formát zadaného emailu';
}
