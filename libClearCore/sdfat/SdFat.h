/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef SdFat_h
#define SdFat_h
/**
 * \file
 * \brief SdFat class
 */
#include "SysCall.h"
#include "BlockDriver.h"
#include "FatLib/FatLib.h"
#include "SdCard/SdioCard.h"
#if INCLUDE_SDIOS
#include "sdios.h"
#endif  // INCLUDE_SDIOS
//------------------------------------------------------------------------------
/** SdFat version 1.1.2 */
#define SD_FAT_VERSION 10102
//==============================================================================
/**
 * \class SdBaseFile
 * \brief Class for backward compatibility.
 */
class SdBaseFile : public FatFile {
 public:
  SdBaseFile() {}
  /**  Create a file object and open it in the current working directory.
   *
   * \param[in] path A path for a file to be opened.
   *
   * \param[in] oflag Values for \a oflag are constructed by a
   * bitwise-inclusive OR of open flags. see
   * FatFile::open(FatFile*, const char*, oflag_t).
   */
  SdBaseFile(const char* path, oflag_t oflag) : FatFile(path, oflag) {}
};
//-----------------------------------------------------------------------------
#if ENABLE_ARDUINO_FEATURES
/**
 * \class SdFile
 * \brief Class for backward compatibility.
 */
class SdFile : public PrintFile {
 public:
  SdFile() {}
  /**  Create a file object and open it in the current working directory.
   *
   * \param[in] path A path for a file to be opened.
   *
   * \param[in] oflag Values for \a oflag are constructed by a
   * bitwise-inclusive OR of open flags. see
   * FatFile::open(FatFile*, const char*, oflag_t).
   */
  SdFile(const char* path, oflag_t oflag) : PrintFile(path, oflag) {}
};
#endif  // #if ENABLE_ARDUINO_FEATURES
//-----------------------------------------------------------------------------
/**
 * \class SdFileSystem
 * \brief Virtual base class for %SdFat library.
 */
template<class SdDriverClass>
class SdFileSystem : public FatFileSystem {
 public:
  /** Initialize file system.
   * \return true for success else false.
   */
  bool begin() {
    return FatFileSystem::begin(&m_card);
  }
  /** \return Pointer to SD card object */
  SdDriverClass *card() {
    m_card.syncBlocks();
    return &m_card;
  }
  /** \return The card error code */
  uint8_t cardErrorCode() {
        return m_card.errorCode();
  }
  /** \return the card error data */
  uint32_t cardErrorData() {
    return m_card.errorData();
  }

 protected:
  SdDriverClass m_card;
};
//==============================================================================
/**
 * \class SdFat
 * \brief Main file system class for %SdFat library.
 */
class SdFat : public SdFileSystem<SdSpiCard> {
 public:
#if IMPLEMENT_SPI_PORT_SELECTION || defined(DOXYGEN)
  SdFat() {
    m_spi.setPort(nullptr);
  }
  /** Constructor with SPI port selection.
   * \param[in] spiPort SPI port number.
   */
  explicit SdFat(CCSPI* spiPort) {
    m_spi.setPort(spiPort);
  }
#endif  // IMPLEMENT_SPI_PORT_SELECTION
  /** Initialize SD card and file system.
   *
   * \param[in] csPin SD card chip select pin.
   * \param[in] spiSettings SPI speed, mode, and bit order.
   * \return true for success else false.
   */
  bool begin(uint8_t csPin = CLEARCORE_PIN_INVALID, uint32_t clockSpeed = SPI_FULL_SPEED) {
    return m_card.begin(&m_spi, csPin, clockSpeed) &&
           SdFileSystem::begin();
  }
  /** Initialize SD card for diagnostic use only.
   *
   * \param[in] csPin SD card chip select pin.
   * \param[in] settings SPI speed, mode, and bit order.
   * \return true for success else false.
   */
  bool cardBegin(uint8_t csPin = CLEARCORE_PIN_INVALID, uint32_t clockSpeed = SPI_FULL_SPEED) {
    return m_card.begin(&m_spi, csPin, clockSpeed);
  }
  /** Initialize file system for diagnostic use only.
   * \return true for success else false.
   */
  bool fsBegin() {
    return FatFileSystem::begin(card());
  }

 private:
  SdFatSpiDriver m_spi;
};
//==============================================================================
#if ENABLE_SDIO_CLASS || defined(DOXYGEN)
/**
 * \class SdFatSdio
 * \brief SdFat class using SDIO.
 */
class SdFatSdio : public SdFileSystem<SdioCard> {
 public:
  /** Initialize SD card and file system.
   * \return true for success else false.
   */
  bool begin() {
    return m_card.begin() && SdFileSystem::begin();
  }
  /** Initialize SD card for diagnostic use only.
   *
   * \return true for success else false.
   */
  bool cardBegin() {
    return m_card.begin();
  }
  /** Initialize file system for diagnostic use only.
   * \return true for success else false.
   */
  bool fsBegin() {
    return SdFileSystem::begin();
  }
};
#if ENABLE_SDIOEX_CLASS || defined(DOXYGEN)
//-----------------------------------------------------------------------------
/**
 * \class SdFatSdioEX
 * \brief SdFat class using SDIO.
 */
class SdFatSdioEX : public SdFileSystem<SdioCardEX> {
 public:
  /** Initialize SD card and file system.
   * \return true for success else false.
   */
  bool begin() {
    return m_card.begin() && SdFileSystem::begin();
  }
  /** \return Pointer to SD card object */
  SdioCardEX* card() {
    return &m_card;
  }
  /** Initialize SD card for diagnostic use only.
   *
   * \return true for success else false.
   */
  bool cardBegin() {
    return m_card.begin();
  }
  /** Initialize file system for diagnostic use only.
   * \return true for success else false.
   */
  bool fsBegin() {
    return SdFileSystem::begin();
  }
};
#endif  // ENABLE_SDIOEX_CLASS || defined(DOXYGEN)
#endif  // ENABLE_SDIO_CLASS || defined(DOXYGEN)
//=============================================================================
#if ENABLE_SOFTWARE_SPI_CLASS || defined(DOXYGEN)
/**
 * \class SdFatSoftSpi
 * \brief SdFat class using software SPI.
 */
template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin>
class SdFatSoftSpi : public SdFileSystem<SdSpiCard>  {
 public:
  /** Initialize SD card and file system.
   *
   * \param[in] csPin SD card chip select pin.
   * \param[in] spiSettings ignored for software SPI..
   * \return true for success else false.
   */
  bool begin(uint8_t csPin = CLEARCORE_PIN_INVALID, uint32_t clockSpeed) {
    return m_card.begin(&m_spi, csPin,clockSpeed) &&
           SdFileSystem::begin();
  }
 private:
  SdSpiSoftDriver<MisoPin, MosiPin, SckPin> m_spi;
};
#endif  // #if ENABLE_SOFTWARE_SPI_CLASS || defined(DOXYGEN)
//==============================================================================
#if ENABLE_EXTENDED_TRANSFER_CLASS || defined(DOXYGEN)
/**
 * \class SdFatEX
 * \brief SdFat class with extended SD I/O.
 */
class SdFatEX : public SdFileSystem<SdSpiCardEX> {
 public:
#if IMPLEMENT_SPI_PORT_SELECTION  || defined(DOXYGEN)
  SdFatEX() {
    m_spi.setPort(nullptr);
  }
  /** Constructor with SPI port selection.
   * \param[in] spiPort SPI port number.
   */
  explicit SdFatEX(CCSPI* spiPort) {
    m_spi.setPort(spiPort);
  }
#endif  // IMPLEMENT_SPI_PORT_SELECTION
  /** Initialize SD card and file system.
  *
  * \param[in] csPin SD card chip select pin.
  * \param[in] spiSettings SPI speed, mode, and bit order.
  * \return true for success else false.
  */
  bool begin(uint8_t csPin = CLEARCORE_PIN_INVALID, uint32_t clockSpeed = SPI_FULL_SPEED) {
    return m_card.begin(&m_spi, csPin, clockSpeed) &&
           SdFileSystem::begin();
  }

 private:
  SdFatSpiDriver m_spi;
};
//==============================================================================
#if ENABLE_SOFTWARE_SPI_CLASS || defined(DOXYGEN)
/**
 * \class SdFatSoftSpiEX
 * \brief SdFat class using software SPI and extended SD I/O.
 */
template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin>
class SdFatSoftSpiEX : public SdFileSystem<SdSpiCardEX>  {
 public:
  /** Initialize SD card and file system.
   *
   * \param[in] csPin SD card chip select pin.
   * \param[in] spiSettings ignored for software SPI.
   * \return true for success else false.
   */
  bool begin(uint8_t csPin = CLEARCORE_PIN_INVALID, uint32_t clockSpeed = SPI_FULL_SPEED) {
    return m_card.begin(&m_spi, csPin, clockSpeed) &&
           SdFileSystem::begin();
  }
 private:
  SdSpiSoftDriver<MisoPin, MosiPin, SckPin> m_spi;
};
#endif  // #if ENABLE_SOFTWARE_SPI_CLASS || defined(DOXYGEN)
#endif  // ENABLE_EXTENDED_TRANSFER_CLASS || defined(DOXYGEN)
//=============================================================================
/**
 * \class Sd2Card
 * \brief Raw access to SD and SDHC card using default SPI library.
 */
class Sd2Card : public SdSpiCard {
 public:
  /** Initialize the SD card.
   * \param[in] csPin SD chip select pin.
   * \param[in] settings SPI speed, mode, and bit order.
   * \return true for success else false.
   */
  bool begin(uint8_t csPin = CLEARCORE_PIN_INVALID, uint32_t clockSpeed = SD_SCK_MHZ(50)) {
    return SdSpiCard::begin(&m_spi, csPin, clockSpeed);
  }
 private:
  SdFatSpiDriver m_spi;
};
#endif  // SdFat_h