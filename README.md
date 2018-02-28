# BMSTU
Academic projects produced during studying at the Moscow State Technical University.

---
To download one folder instead of entire repository using Subversion (from [here](https://stackoverflow.com/questions/7106012/download-a-single-folder-or-directory-from-a-github-repo/18194523#18194523)):
1. Copy folder address.
2. Replace `tree/master` part with `trunk` in the URL.
3. Use command `svn export <modified URL>` for dowloading.

I.g. we want to download folder [https://github.com/nastusha-merry/BMSTU/tree/master/Modelling](https://github.com/nastusha-merry/BMSTU/tree/master/Modelling).

So we need to tap `svn export https://github.com/nastusha-merry/BMSTU/trunk/Modelling>` to command line.
