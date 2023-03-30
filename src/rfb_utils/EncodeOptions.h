#pragma once
#include <vector>

class EncodeOptions
{
public:
  EncodeOptions();
  virtual ~EncodeOptions();

  void reset();
  void setEncodings(std::vector<int>* list);

  int getPreferredEncoding() const;
  bool encodingEnabled(int code) const;
  int getCompressionLevel(int defaultLevel = EO_DEFAULT) const;

  int getJpegQualityLevel(int defaultLevel = EO_DEFAULT) const;

  bool jpegEnabled() const;
  void disableJpeg();

  bool copyRectEnabled() const;
  bool richCursorEnabled() const;
  bool pointerPosEnabled() const;
  bool desktopSizeEnabled() const;


protected:
  static bool normalEncoding(int code);

  static const int EO_DEFAULT = -1;

  int m_preferredEncoding;

  bool m_enableRRE;
  bool m_enableHextile;
  bool m_enableZrle;
  bool m_enableTight;

  int m_compressionLevel;
  int m_jpegQualityLevel;

  bool m_enableCopyRect;
  bool m_enableRichCursor;
  bool m_enablePointerPos;
  bool m_enableDesktopSize;

};

