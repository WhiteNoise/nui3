/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiFontBase;

enum nuiTextDirection
{
  nuiLeftToRight,
  nuiRightToLeft,
  nuiTopToBottom,
  nuiBottomToTop
};

enum nuiTextBaseline
{
  nuiTextBaselineNormal,
  nuiTextBaselineSuperScript,
  nuiTextBaselineSubScript
};

class nuiTextStyle
{
public:
  nuiTextStyle();
  nuiTextStyle(const nuiTextStyle& rStyle);
  ~nuiTextStyle();
  
  nuiTextStyle& operator=(const nuiTextStyle& rStyle);
  
  void SetFont(nuiFontBase* pFont);
  nuiFontBase* GetFont() const;
  void SetDensity(nuiSize X, nuiSize Y);
  nuiSize GetDensityX() const;
  nuiSize GetDensityY() const;
  void SetSpacesPerTab(int count);
  int32 GetSpacesPerTab();
  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;
  void SetColor(const nuiColor& rColor);
  const nuiColor& GetColor() const;
  bool UseColor() const;
  void SetUseColor(bool set);
  void SetBaseline(nuiTextBaseline set);
  nuiTextBaseline GetBaseline() const;
  void SetDirection(nuiTextDirection set);
  nuiTextDirection GetDirection() const;
  void SetJustify(bool set);
  bool GetJustify() const;
  
private:
  nuiFontBase* mpFont;
  float mDensityX;
  float mDensityY;
  int32 mSpacesPerTab;
  nuiColor mColor;
  bool mUseColor : 1;
  bool mUnderline : 1;
  bool mStrikeThrough : 1;
  nuiTextBaseline mBaseline;
  bool mJustify : 1;
  nuiTextDirection mDirection;
};

