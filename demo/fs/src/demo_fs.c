


extern void FS_OldTest();
extern void FS_NewTest();



void fs_Main()
{
#ifdef FS_USE_OLD_VERSION
    API_FS_SetUseOldVersion(true);
    FS_OldTest();
#else
    FS_NewTest();
#endif
    
}



