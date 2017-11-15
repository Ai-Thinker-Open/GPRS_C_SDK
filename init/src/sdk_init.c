


#include <sdk_init.h>
/*#########################################################################################*/
/*please don't change anything in the following function*/

void user_Init(T_INTERFACE_VTBL_TAG *pVtable);
T_INTERFACE_VTBL_TAG * g_InterfaceVtbl = 0;
typedef struct T_VTBL_TAG
{
    const UINT32 magic;
    void (*user_init)(T_INTERFACE_VTBL_TAG *pVtable);
}T_VTBL_TAG;
static const volatile T_VTBL_TAG __attribute__((__section__ (".section_init")))
g_s_UserVtbl   =
{
    (UINT32)0x87654321,
    user_Init,
    // user_main,
};//
volatile T_VTBL_TAG* temp;
void user_Main(void)
{
    temp = &g_s_UserVtbl.magic;
    PRONAME_MAIN();
}
void user_Init(T_INTERFACE_VTBL_TAG *pVtable)
{
    g_InterfaceVtbl = pVtable;
    user_Main();
}
/*#########################################################################################*/






