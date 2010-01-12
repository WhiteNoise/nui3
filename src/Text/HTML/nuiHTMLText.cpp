/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLText.h"

///////////////////////////////////////// nuiHTMLText
nuiHTMLText::nuiHTMLText(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, const nglString& rText)
: nuiHTMLItem(pNode, pAnchor, true), mText(rText), mpLayout(NULL), mpFont(NULL), mpCompositeLayout(NULL), mpNextInRun(NULL), mFirstInRun(false), mUnderline(false), mStrikeThrough(false)
{

}

nuiHTMLText::~nuiHTMLText()
{
  if (mpFont)
    mpFont->Release();
  delete mpLayout;
  delete mpCompositeLayout;
  mpNextInRun = NULL;
}

void nuiHTMLText::Draw(nuiDrawContext* pContext)
{
  if (!mFirstInRun)
    return;
  
  pContext->SetTextColor(mTextFgColor);
  //nuiColor mTextBgColor;
  pContext->SetFont(mpFont, false);
  
  if (!mpCompositeLayout)
  {
    nuiHTMLText* pIt = this;
    nglString str(nglString::Empty);
    do 
    {
      if (!pIt->mFirstInRun)
        str.Add(_T(" "));
      str.Add(pIt->mText);
      pIt = pIt->mpNextInRun;
    } while (pIt);
      
    mpCompositeLayout = new nuiFontLayout(*mpFont, 0, 0, nuiHorizontal);
    mpCompositeLayout->SetUnderline(mUnderline);
    mpCompositeLayout->SetStrikeThrough(mStrikeThrough);
    mpCompositeLayout->Layout(str);
  }
  
  pContext->DrawText(0, mpCompositeLayout->GetAscender() , *mpCompositeLayout);
}

void nuiHTMLText::Layout(nuiHTMLContext& rContext)
{
  delete mpLayout;
  if (mpFont)
    mpFont->Release();
  mpFont = rContext.mpFont;
  mpFont->Acquire();
  
  mpLayout = new nuiFontLayout(*mpFont, 0, 0, nuiHorizontal);
  mpLayout->SetUnderline(rContext.mUnderline);
  mpLayout->SetStrikeThrough(rContext.mStrikeThrough);
  mUnderline = rContext.mUnderline;
  mStrikeThrough = rContext.mStrikeThrough;
  
  mpLayout->Layout(mText);
  mIdealRect = mpLayout->GetRect();
  mIdealRect.SetWidth(mIdealRect.GetWidth() + rContext.mHSpace);
  mTextFgColor = rContext.mTextFgColor;
  mTextBgColor = rContext.mTextBgColor;
  //printf("text layout done (%ls)\n", mIdealRect.GetValue().GetChars());
  
  delete mpCompositeLayout;
  mpCompositeLayout = NULL;
}

float nuiHTMLText::GetAscender() const
{
  return mpLayout->GetAscender();
}

float nuiHTMLText::GetDescender() const
{
  return mpLayout->GetDescender();
}

void nuiHTMLText::SetNextInRun(nuiHTMLText* pNext)
{
  mpNextInRun = pNext;
}

void nuiHTMLText::SetFirstInRun(bool set)
{
  mFirstInRun = set;
}
