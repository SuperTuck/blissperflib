# BlissPerfLib #

A small and easy to use performance library (.dll) for bliss (dayz server).

So why it makes sense to optimize server script code sometimes?  

We are talking here mainly about so called eventhandlers (EH's).  
EH's are scripted procedures/function wich are called on demand. For example, an EH can be executed if a public variable change. EH are currently running in the so called non scheduled environment in a **single thread**. This means there is only **one** running EH at the same time. So if one slow running EH is executed, all other events must wait in a queue.
To have a fast responding server it is important, that we write our EH code with highest possible efficiency.

Now lets explain what `blissperflib.dll` can do for us and how it is to use.

Essential part of this library is the included profiler. It works by collecting call statistics for code segments and logs all call statistic data to a logfile. All this works in realtime and logging works asynchron (in separat thread), so the profiler has nearly no impact to overall performance.
Just lets run your prepared server with high load for a few hours and then evaluate `blissperflib.log`. So you can easily spot the bottlenecks and then start investigating the related code. 

In some cases it is useful or nessecary to replace script code by much faster c code.  
This library supports you in this case too, by providing an easy to use interface template.   
In the current bliss implementation, we can find a perfect example of such a bottleneck:  
The function `fnc\_split`, wich is at least 2 times called for every call to hiveWrite and hiveReadWrite, and have to process very long strings.  

Look at the script code of `fnc\_split` what it does and compare it with the related c code.  
The related c function is here more then 100 times faster, especially if we have player with large inventory.  

Realize that if you have 30 players and more this is a really important advantage and makes your server faster responding and freeing much server resources too.


**How to use details and links:**

**Installation:**  

Compile `blissperflib.dll` from source `/src` or get the library binary (`.dll`) from here:  

[https://github.com/fred41/blissperflib/blob/master/compiled/blissperflib.dll](https://github.com/fred41/blissperflib/blob/master/compiled/blissperflib.dll)
  
Copy `blissperflib.dll` to your **Arma 2** directory.
   
To obtain function/codesegment names in `blissperflib.log` instead of numbers, there must be a blissperflib.ini file in Arma 2 directory (see below for syntax).

**Profiler:**  
To profile a script (or piece of code),	just insert the following lines at begin and end:  

`_PCID19 = "blissperflib" callExtension "I:19";`   
`...code...`    
`...code...`    
`"blissperflib" callExtension format ["O:%1",_PCID19];`  

Where **all** `'19'` represents a unique ID and must be replaced with a integer in range 10..99  
(Make sure that in all .sqf functions, the **return variable** is always the last line!!!)  

To show real names in the log, there must be valid entrys in blissperflib.ini  
example of `blissperflib.ini`:  

`[profiler]`    
`18 = server_hiveReadWrite`   
`19 = server_hiveWrite`  
`20 = server_onPlayerConnect`   

To save much work there is a perl script prepprof.pl (prepare profiler) here:

[https://github.com/fred41/blissperflib/blob/master/perl/prepprof.pl](https://github.com/fred41/blissperflib/blob/master/perl/prepprof.pl)

It generates `blissperflib.ini` for you and inserts the two profiler code lines at begin and end in every .sqf file from a specified directory for you. A little handwork is nessecary, if you have functions in this directory, wich need the return variable as last line. Here, after running the script, you just have to move the return variable manually to the last line.  
The default directory where the script is looking for .sqf files is `./bliss/dayz_server/compile`. There are mainly the server side event handlers placed. To change it currently you have to change it in `prepprof.pl`.  
If you are ready with profiling work, just restore the original files from repository or your backup. 


The profiler logging output goes to `Arma2Dir\blissperflib.log` and a line looks like that:  

`12:23:59 PT:   1 TC:      37 AD:    6.07ms    21.14ms : fnc_split`   

`PT: currently parallel running pseudo threads (only investigated code counts)`  
`TC: total count of this function/codesegment since server start`  
`AD: average duration of this function/codesegment`  
`CD: current duration of this function/codesegment`  
`and last the name of this function/codesegment`  


**fnc\_split:** (the first black sheep found)  
`fnc\_split` is the first function replacment, implemented in this library (`blissperflib.dll`).   

It is **more then 100 times faster!!!** then the related .sqf code,  
so this replacement will realy relax your server ressources.  

If you have a big server with 30+ players, such optimization is noticeable for you and your players.

If you only want use this library function just do the following:  
Replace the file `Repository\bliss\dayz_server\compile\fnc_split.sqf` with the file from here:  

[https://github.com/fred41/blissperflib/blob/master/src/fnc_split.sqf](https://github.com/fred41/blissperflib/blob/master/src/fnc_split.sqf)

Run repack.pl from your bliss repository and copy `Repository\deploy\@Bliss\addons\dayz_server.pbo`   
to `Arma 2\@Bliss\addons\dayz_server.pbo`  

Just to clarify, it is **NOT NESSECARY** to prepare the profiler, if you only want use the optimized function('s) from the library.   
All you need then is the installed `blissperflib.dll` and the related `.sqf` replacement, `fnc_split.sqf` for example. Thats all for now.

**ENJOY THE DIFFERENCE** and sorry for my english :/

Special thanks to ayan4m1 and the bliss team, for the great bliss development:  
[http://dayzprivate.com/](http://dayzprivate.com/)