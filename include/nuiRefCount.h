/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

class nuiRefCount
{
public:
  nuiRefCount() : mCount(0), mPermanent(false) , mTrace(false)
  {
  }

  virtual ~nuiRefCount()           
  { 
    //NGL_ASSERT(mCount == 0); 
  }

  uint32 Acquire() const   
  { 
//    if (mTrace)
//    {
//      NGL_OUT(_T("Acquire object 0x%x (%d)\n"), this, mCount + 1);
//    }
    return ++mCount; 
  }

  uint32 Release()
  { 
//    if (mTrace)
//    {
//      NGL_OUT(_T("Release object 0x%x (%d)\n"), this, mCount - 1);
//    }
    NGL_ASSERTR(mCount > 0, mCount); 
    mCount--;
    if (mCount == 0)
    {
//      if (mTrace)
//      {
//        NGL_OUT(_T("Delete object 0x%x\n"), this);
//      }
      OnFinalize();
      delete this;
      return 0;
    }
    return mCount; 
  }

  uint32 GetRefCount()
  {
    return mCount;
  }

  void SetPermanent(bool Permanent = true)
  {
    if (Permanent)
    {
      if (mPermanent)
        return;
      Acquire();
      mPermanent = true;
    }
    else
    {
      if (!mPermanent)
        return;
      mPermanent = false;
      Release();
    }
  }

  bool IsPermanent()
  {
    return mPermanent;
  }

  virtual void OnFinalize() ///< This callback is called when we are about to delete this
  {
  }
  
protected:
  bool mTrace;
private:
  mutable uint32 mCount;
  bool mPermanent;
};

