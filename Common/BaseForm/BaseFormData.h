/*!
\author luyj 2016.11.15
\section 窗体基类数据类
*/
#ifndef BASEFORMDATA_H
#define BASEFORMDATA_H

class CBaseFormData
{
public:
    CBaseFormData();
public:
    bool m_modify;      //是否修改
    bool m_isActived;   //是否激活
    bool m_needScroll;  //是否需要滚动条
    bool m_modifiedVisible;//修改标记是否可见

    int m_pwid; // 外部父窗体,未设置时默认值-1
};

#endif // BASEFORMDATA_H
