# blissperflib #

A small performance library (.dll) for bliss (dayz server).

The main intention of this temporarily development is to improve the performance of dayz server,
by replacing slow, resource hungry parts of script code, with c code.

The second purpose is to provide an easy way for profiling (performance measure) Arma 2 
script code, to find hotspots/bottlenecks.

It makes use of callExtension, the IPC interface for Arma 2.

**How to use:**

**Installation:**  

Compile *blissperflib.dll* from source /src or get this from here:  
  
Copy *blissperflib.dll* to your **Arma 2** directory.   
If you need function/codepart names in *blissperflib.log*, copy blissperflib.ini to **Arma 2** directory or create your own here.

**Profiler:**  
To profile a script (or piece of code), just insert at begin and end the following lines:  

_PCID**##** = "profiler" callExtension "I:**##**";   
...code...    
...code...    
"profiler" callExtension format ["O:%1",_PCID**##**];  

Where **##** represents a unique ID and is to replace with a integer in range 0..255  
(Make sure that in functions, the **return variable** is always the last line!!!)  

To show real names in the log, there must be valid entrys in blissperflib.ini  
example of blissperflib.ini:  

[profiler]    
23 = server_hiveWrite   
24 = server_hiveReadWrite  
25 = fnc_split   

The profiler output goes to Arma2Dir\blissperflib.log and looks like that:  

`12:23:59 PT:   1 TC:      37 AD:    6.07ms    21.14ms : fnc_split`   

PT: currently parallel threads (pseudo threads)  
TC: total count of this function/codepart since server start  
AD: average duration of this function/codepart  
CD: current duration of this function/codepart  
and last the name of this function/codepart  


**fnc\_split:**  
*fnc\_split* is the first function replaced by a very fast c function included in this library (blissperflib.dll).   

For big inventory strings, it is more then 100 times faster!!! then the .sqf code,   
so this replacemnet will significant relax your server ressources.  

To use its improvement just do the following:  
Replace the file *Repository\bliss\dayz_server\compile\fnc_split.sqf* with the file from here:  

[https://github.com/fred41/DayZ-Private/blob/master/bliss/dayz_server/compile/fnc_split.sqf](https://github.com/fred41/DayZ-Private/blob/master/bliss/dayz_server/compile/fnc_split.sqf)  

Run repack.pl from your bliss repository and copy *Repository\deploy\@Bliss\addons\dayz_server.pbo*   
to *Arma 2\@Bliss\addons\dayz_server.pbo*  

Feedbacks are wellcome.  
