# qtFileEncrypter
Simplest file encryption program imaginable using C++ Qt

Encrypts your file with threefish 256 algorithm, using the sha256 hash of your password as a key 

## TL;DR
**Don't use it for any actually important data, the only reason it now has this repo on github, is for potential employers to see an example of my work**

## Obvious shortcomings
- I made this thing almost 2 years ago as a *fun* project for a couple of days, and didn't put much thought into it
- Output file has no metadata, not even hash of the source file, thus, while decrypting, the program can't detect if the password is wrong, and if an incorrect password is provided you'll just get garbage data output
- Even tho i made the program theoretically independent of the endian of the machine, i never actually tested it outside of x86

If you find something I should also mention in the *shortcomings* section, let me know, may be you'll help me become a better developer. **Cheers ; )**
