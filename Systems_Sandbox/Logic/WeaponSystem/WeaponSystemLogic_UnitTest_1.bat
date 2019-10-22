REM : SAFTEY, FIRE, RELOAD, FIRE; per trigger pulls
@echo off
(
echo hello

echo s
echo d
echo t

echo f
echo 1

echo s
echo d
echo t

echo r

echo s
echo d
echo t

echo f
echo 1

echo s
echo d
echo t

echo q
) | WeaponSystemLogic.exe

REM Allow for way to comment out 'q' and to have the batch cease running
REM GOAL: Have batch run desired commands so testing can be done at
REM       the point where the script leaves off where it finishes