<?

//
// index.php
//
// server entry point
//

?>

<html>
<body>

<?
// otherwise generate the upload form
require("form.php");

// if a file upload has been requested
if (isset($_FILES["MyFile"]))
{
  require("upload.php");
  exit();
}

$data = @file_get_contents('php://input');  
if ($data)
{
//    echo "<br/>\n<br/>\n<br/>\n<br/>\n".$data;
}

function ParseArray($myarray, $indent)
{
  foreach ($myarray as $item_key => $item_value)
  {
    if ($item_key == "GLOBALS")
      continue;
        
    for ($i = 0; $i < $indent; $i++)
      echo "&nbsp;";
    
    if (is_array($item_value))
    {
      echo "<br/>\n<br/>\n".$item_key." : ARRAY<br/>\n-------------------------------------------<br/>\n";
      ParseArray($item_value, $indent+0);
    }
    else
    {
      echo $item_key." : ".$item_value."<br/>\n";
    }
  }  
}

//ParseArray($GLOBALS, 0);

exit();

?>

</body>
</html>



