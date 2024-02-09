# Valkyria_Tools
Tools For Valkyria's Engine, .sdt .dat .mg2

## Tools
### SDT_TextEditor
 - export text from `.sdt` to json
 - import text from json to `.sdt`
> Q: ***Can it be longer than the original text?***  
> A: yes, but the maximum engine text buffer is 256 bytes.

### DAT_Editor
 - unpack `.dat`
 - repack `.dat`
> Q: ***How to make a patch?***  
> A: if the original .dat file name is `data06-00.dat`, just pack the files that need to replace as new pack name `data06-01.dat` or `data06-02.dat` etc.

