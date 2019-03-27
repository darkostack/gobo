#ifndef CODE_UTILS_H_
#define CODE_UTILS_H_

#define goEXPECT(aCondition) \
    do                       \
    {                        \
        if (!(aCondition))   \
        {                    \
            goto exit;       \
        }                    \
    } while (0)

#define goEXPECT_ACTION(aCondition, aAction) \
    do                                       \
    {                                        \
        if (!(aCondition))                   \
        {                                    \
            aAction;                         \
            goto exit;                       \
        }                                    \
    } while (0)

#endif // CODE_UTILS_H_
