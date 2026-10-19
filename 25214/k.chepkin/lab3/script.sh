touch file.txt
gcc -Wall -Wextra main.c -o prog
chmod u+r,u+w,u+x,g-r,g-w,g+x,o-r,o-w,o-x prog
echo "------------------"
echo "Launched the rogram with execution rights only set for owner and their group:"
ls -l | grep prog
./prog
echo "------------------"
chmod u+s prog
echo "Launched the program with setuid bit:"
ls -l | grep prog
./prog

# 1) Some user from the same group as me launches this script.
# 2) It attempts to launch the "prog" binary.
# 3) The binary executes successfully.
# 4) But the binary is unable to modify "file.txt" file because neither its EUID nor UID have permission to.
# 5) Then this script sets SUID bit of the "file.txt" on.
# 6) And reruns the "prog" binary.
# 7) Now said binary runs with EUID of my user and is able to modify "file.txt" (first fprintf).
# 8) But the binary itself changes its EUID to match UID with "setuid" syscall.
# 9) This takes away its permission to modify "file.txt" and next attempt (to run fprintf) fails.