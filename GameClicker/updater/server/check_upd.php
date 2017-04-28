<?php

echo("<xml>\n");
if ($handle = opendir('./downloads/files')) {
    /* This is the correct way to loop over the directory. */
    while (false !== ($file = readdir($handle))) {
        if ($file == ".." || $file==".") continue;
        echo "<file name='$file'>\n";
        echo "  <link>http://igra-em.ru/downloads/files/$file</link>\n";
        echo "  <md5>";
        echo md5_file("./downloads/files/$file");
        echo "</md5>\n";
        echo "  <size>";
        echo sprintf("%u", filesize("./downloads/files/$file"));
        echo "</size>\n";
        echo "</file>\n";

    }
    closedir($handle);
}
echo("</xml>\n");
?>