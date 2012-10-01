private ["_split"];

_split = "blisshive" callExtension format["S:%1~#~%2",(_this select 0),(_this select 1)];
call compile format ["_split = %1", _split];

_split;
