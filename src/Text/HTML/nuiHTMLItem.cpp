/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "nuiHTMLItem.h"
#include "nuiHTMLBox.h"

/////////////////class nuiHTMLItem
nuiHTMLItem::nuiHTMLItem(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, bool Inline)
: mpNode(pNode),
  mInline(Inline),
  mEndTag(false),
  mLineBreak(false),
  mpParent(NULL),
  mpAnchor(pAnchor),
  mVisible(true),
  mSetRectCalled(false),
  mpInlineStyle(NULL)
{
  ForceLineBreak(pNode->GetTagType() == nuiHTMLNode::eTag_BR);
  nuiHTMLAttrib* pStyle = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_STYLE);
  if (pStyle)
  {
    AddStyleSheet(mpNode->GetSourceURL(), pStyle->GetValue(), true);
  }
}

nuiHTMLItem::~nuiHTMLItem()
{
  for (uint32 i = 0; i < mStyleSheets.size(); i++)
  {
    delete mStyleSheets[i];
  }
  
  delete mpInlineStyle;
}

void nuiHTMLItem::Draw(nuiDrawContext* pContext)
{
}

uint32 nuiHTMLItem::GetDepth() const
{
  if (mpParent)
    return 1 + mpParent->GetDepth();
  return 0;
}

void nuiHTMLItem::CallDraw(nuiDrawContext* pContext)
{
  if (!mVisible)
    return;

#if 0
  for (uint32 i = 0; i < GetDepth(); i++)
    printf("  ");
  nglString id;
  nuiHTMLAttrib* pAttrib = mpNode->GetAttribute(nuiHTMLAttrib::eAttrib_ID);
  if (pAttrib)
    id.Add(_T(" id='")).Add(pAttrib->GetValue()).Add(_T("'"));
  printf("nuiHTMLItem::CallDraw <%ls%ls> %ls\n", mpNode->GetName().GetChars(), id.GetChars(), mRect.GetValue().GetChars());
#endif
  
  NGL_ASSERT(mSetRectCalled);
  pContext->PushMatrix();
  pContext->Translate(mRect.Left(), mRect.Top());
  //pContext->DrawRect(GetRect().Size(), eStrokeShape);
  Draw(pContext);
  pContext->PopMatrix();
}

void nuiHTMLItem::Layout(nuiHTMLContext& rContext)
{
  bool set = !IsEndTag();
  switch (mpNode->GetTagType())
  {
  case nuiHTML::eTag_I:
  case nuiHTML::eTag_EM:
    {
      rContext.mFont.SetItalic(set, 1.0f);
      rContext.UpdateFont();
    }
    break;
  case nuiHTML::eTag_B:
  case nuiHTML::eTag_STRONG:
    {
      rContext.mFont.SetBold(set, 1.0f);
      rContext.UpdateFont();
    }
    break;
  case nuiHTML::eTag_U:
    {
      rContext.mUnderline = set;
    }
    break;
  case nuiHTML::eTag_STRIKE:
    {
      rContext.mUnderline = set;
    }
    break;
  case nuiHTML::eTag_A:
    {
      rContext.mUnderline = set;
      if (set)
      {
        mOldTextColor = rContext.mTextFgColor;
        rContext.mTextFgColor = rContext.mLinkColor;
      }
      else
      {
        rContext.mTextFgColor =  mOldTextColor;
      }
    }
    break;
  }
}

const nuiRect& nuiHTMLItem::GetRect() const
{
  return mRect;
}

void nuiHTMLItem::SetRect(const nuiRect& rRect)
{
  mSetRectCalled = true;
  mRect = rRect;
}

#undef max

const nuiRect& nuiHTMLItem::GetIdealRect() const
{
  return mIdealRect;
}

void nuiHTMLItem::MoveTo(float x, float y)
{
  mRect.MoveTo(x, y);
}

void nuiHTMLItem::SetWidth(float w)
{
  mRect.SetWidth(w);
}

void nuiHTMLItem::SetHeight(float h)
{
  mRect.SetHeight(h);
}

nuiHTMLNode* nuiHTMLItem::GetNode() const
{
  return mpNode;
}

void nuiHTMLItem::SetParent(nuiHTMLItem* pBox)
{
  mpParent = pBox;
}

nuiHTMLItem* nuiHTMLItem::GetParent() const
{
  return mpParent;
}

float nuiHTMLItem::GetAscender() const
{
  return mIdealRect.GetHeight();
}

float nuiHTMLItem::GetDescender() const
{
  return 0;
}

bool nuiHTMLItem::IsInline() const
{
  return mInline;
}

void nuiHTMLItem::SetInline(bool set)
{
  mInline = set;
}

bool nuiHTMLItem::IsEndTag() const
{
  return mEndTag;
}

void nuiHTMLItem::SetEndTag(bool set)
{
  mEndTag = set;
}

bool nuiHTMLItem::IsLineBreak() const
{
  return mLineBreak;
}

void nuiHTMLItem::ForceLineBreak(bool s)
{
  mLineBreak = s;
}

nglString nuiHTMLItem::GetAbsoluteURL(const nglString& rString) const
{
  nglString url(rString);
  nuiHTML::GetAbsoluteURL(mpNode->GetSourceURL(), url);
  return url;
}

bool nuiHTMLItem::IsInside(float X, float Y) const
{
  return mRect.IsInside(X, Y);
}

void nuiHTMLItem::GetItemsAt(std::vector<nuiHTMLItem*>& rHitItems, float X, float Y) const
{
  if (IsInside(X, Y))
    rHitItems.push_back(const_cast<nuiHTMLItem*>(this));
}

nuiRect nuiHTMLItem::GetGlobalRect() const
{
  nuiRect r;
  if (mpParent)
    r = mpParent->GetGlobalRect(); 
  r.Move(mRect.Left(), mRect.Top());
  r.SetSize(mRect.GetWidth(), mRect.GetHeight());
  return r;
}

void nuiHTMLItem::SetAnchor(nuiHTMLNode* pAnchor)
{
  mpAnchor = pAnchor;
}

nuiHTMLNode* nuiHTMLItem::GetAnchor() const
{
  return mpAnchor;
}

void nuiHTMLItem::UpdateVisibility(const nuiRect& rVisibleRect)
{
  nuiRect r;
  mVisible = r.Intersect(GetGlobalRect(), rVisibleRect);
}

void nuiHTMLItem::Invalidate()
{
  if (mpParent)
    mpParent->Invalidate();
  if (mDisplayChangedDelegate)
    mDisplayChangedDelegate();
}

void nuiHTMLItem::SetLayout(const nuiRect& rRect)
{
  nuiRect r(rRect);
  r.RoundToBiggest();
  if (mSetRectCalled && r.GetWidth() == mRect.GetWidth() & r.GetHeight() == mRect.GetHeight())
  {
    MoveTo(r.Left(), r.Top());
  }
  else
  {
    SetRect(r);
  }

}


void nuiHTMLItem::InvalidateLayout()
{
  mSetRectCalled = false;
  if (mpParent)
    mpParent->InvalidateLayout();
  if (mLayoutChangedDelegate)
    mLayoutChangedDelegate();
}

void nuiHTMLItem::SetLayoutChangedDelegate(const nuiFastDelegate0<>& rDelegate)
{
  mLayoutChangedDelegate = rDelegate;
}

void nuiHTMLItem::SetDisplayChangedDelegate(const nuiFastDelegate0<>& rDelegate)
{
  mDisplayChangedDelegate = rDelegate;
}

void nuiHTMLItem::StyleSheetDone(nuiCSSStyleSheet* pCSS)
{
  NGL_ASSERT(pCSS);
  if (!pCSS->IsValid())
  {
    delete pCSS;
    for (uint32 i = 0; i < mStyleSheets.size(); i++)
    {
      if (mStyleSheets[i] == pCSS)
      {
        mStyleSheets.erase(mStyleSheets.begin() + i);
        return;
      }
    }
  }
  
  InvalidateLayout();
}

void nuiHTMLItem::AddStyleSheet(const nglString& rBaseURL, const nglString& rText, bool Inline)
{
  nuiCSSStyleSheet* pCSS = nuiCSSEngine::CreateStyleSheet(rBaseURL, rText, Inline, nuiMakeDelegate(this, &nuiHTMLItem::StyleSheetDone));
  if (pCSS)
  {
    if (Inline)
    {
      delete mpInlineStyle;
      mpInlineStyle = pCSS;
    }
    else
      mStyleSheets.push_back(pCSS);
  }
}

void nuiHTMLItem::AddStyleSheet(const nglString& rURL)
{
  nuiCSSStyleSheet* pCSS = nuiCSSEngine::CreateStyleSheet(rURL, nuiMakeDelegate(this, &nuiHTMLItem::StyleSheetDone));
  if (pCSS)
  {
    mStyleSheets.push_back(pCSS);
  }
}

const std::vector<nuiCSSStyleSheet*>& nuiHTMLItem::GetStyleSheets() const
{
  return mStyleSheets;
}

const nuiCSSStyleSheet* nuiHTMLItem::GetInlineStyle() const
{
  return mpInlineStyle;
}

int32 nuiHTMLItem::GetChildrenCount() const
{
  return 0;
}

nuiHTMLItem* nuiHTMLItem::GetChild(int32 index) const
{
  return NULL;
}


