# blissperflib #

A small and easy to use performance library (.dll) for bliss (dayz server).

The main intention of this development is to improve the performance of dayz server,  
by replacing slow, resource hungry parts of script code, with c code.  
Doing that on the rigth place in script code, you can get large improvements in performance.  
The first step is to find the **bottlenecks** , that are frequently used and/or heavy proccesing parts in your script code.   
A perfect example is the function **fnc\_split** wich is at least 2 times called for every
call to hiveWrite and have to process very long strings.  
Look at script code of **fnc\_split** what it does and compare it with the related c code.  
The c function is here more then 100 times faster, if we have player with large inventory.  
Keep in mind that if you have 50 players and more this may be the improvement you need to stabelize your server for long time.

A simple profiler with logging feature is included in this library, to provide an easy way for profiling Arma 2 script code.

The library functions are called via **callExtension**, the IPC interface for Arma 2.

**How to use:**

**Installation:**  

Compile *blissperflib.dll* from source /src or get the library binary (.dll) from here:  

[https://github.com/fred41/blissperflib/blob/master/compiled/blissperflib.dll](https://github.com/fred41/blissperflib/blob/master/compiled/blissperflib.dll)
  
Copy *blissperflib.dll* to your **Arma 2** directory.
   
If you need function/codepart names in *blissperflib.log*, copy cfg/blissperflib.ini to **Arma 2** directory or create your own (see below for syntax).


**Profiler:**  
To profile a script (or piece of code),	just insert the following lines at begin and end:  

`_PCID25 = "blissperflib" callExtension "I:25";`   
`...code...`    
`...code...`    
`"blissperflib" callExtension format ["O:%1",_PCID25];`  

Where all **'25'** represents a unique ID and must be replaced with a integer in range 0..255  
(Make sure that in all .sqf functions, the **return variable** is always the last line!!!)  

To show real names in the log, there must be valid entrys in blissperflib.ini  
example of blissperflib.ini:  

`[profiler]`    
`23 = server_hiveWrite`   
`24 = server_hiveReadWrite`  
`25 = fnc_split`   

Get a example here:  

[https://github.com/fred41/blissperflib/blob/master/cfg/blissperflib.ini](https://github.com/fred41/blissperflib/blob/master/cfg/blissperflib.ini)

The profiler output goes to Arma2Dir\blissperflib.log and a line looks like that:  

`12:23:59 PT:   1 TC:      37 AD:    6.07ms    21.14ms : fnc_split`   

`PT: currently parallel threads (pseudo threads)`  
`TC: total count of this function/codepart since server start`  
`AD: average duration of this function/codepart`  
`CD: current duration of this function/codepart`  
`and last the name of this function/codepart`  


**fnc\_split:**  
*fnc\_split* is the first function replaced by a very fast c function included in this library (blissperflib.dll).   

For big inventory strings, it is **more then 100 times faster!!!** then the .sqf code,   
so this replacement will realy relax your server ressources.  

If you have a big server with 30+ players, i mean this is a **MUST**.

To use this library function just do the following:  
Replace the file *Repository\bliss\dayz_server\compile\fnc_split.sqf* with the file from here:  

[https://github.com/fred41/blissperflib/blob/master/src/fnc_split.sqf](https://github.com/fred41/blissperflib/blob/master/src/fnc_split.sqf)

Run repack.pl from your bliss repository and copy *Repository\deploy\@Bliss\addons\dayz_server.pbo*   
to *Arma 2\@Bliss\addons\dayz_server.pbo*  

**ENJOY THE DIFFERENCE** and sorry for my english :/

Special thanks to ayan4m1 and the bliss team, for the great bliss development:  
[http://dayzprivate.com/](http://dayzprivate.com/)