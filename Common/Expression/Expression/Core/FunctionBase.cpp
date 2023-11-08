#include "FunctionBase.h"


CFunctionBase::~CFunctionBase()
{

}

void CFunctionBase::Release()
{
    //for(IEmpty* p : m_args)
    for(int i = 0 ;i < m_args.count();i++)
    {
        IEmpty* p = m_args[i];
        if(p->inHerits("CFunctionBase"))
        {
            ((CFunctionBase*)p)->Release();
        }
            delete p;
    }
    m_args.clear();
}

CFunctionBase *CFunctionBase::Clone()
{
    CFunctionBase* f = create();
    //for (IEmpty* v : m_args)
    for(int i = 0 ;i < m_args.count();i++)
    {
      IEmpty* v = m_args[i];
      if (v->inHerits("CFunctionBase"))
      {
          CFunctionBase* fb = (CFunctionBase*)v;
          bool isok = true;
          f->pushArg(fb->Clone(),isok);
          if(!isok)
              return nullptr;
      }
      else
      {
          bool isok = true;
          f->pushValue(v->toString(),isok);
          if(!isok)
              return nullptr;
      }
    }
    return f;
}


IEmpty *CFunctionBase::popArg()
{
    int last = m_args.length()-1;
    if(last >=0)
    {
        IEmpty* i = m_args.at(last);
        m_args.removeAt(last);
        return i;
    }
    throw"functionbase.pop";
}

bool CFunctionBase::operator ==(CFunctionBase *other)
{
    throw"functionbase.operator ==";
}

bool CFunctionBase::operator !=(CFunctionBase *other)
{
    throw"functionbase.operator !=";
}
