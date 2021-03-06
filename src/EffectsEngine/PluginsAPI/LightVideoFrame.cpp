/*****************************************************************************
 * LightVideoFrame.cpp: class used to contains raw video frame, with
 * copy-on-write and references conter mecanism for performance and avoiding
 * memory leak
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
 *
 * Authors: Vincent Carrubba <cyberbouba@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "LightVideoFrame.h"

#include <QSharedData>
#include <QDebug>
#include <qmutex.h>
#include <QWriteLocker>
#include <QReadLocker>

VideoFrame::~VideoFrame()
{
  if ( frame.octets != NULL )
    delete [] frame.octets;
}

VideoFrame::VideoFrame( void )
{
  frame.octets = NULL;
  nboctets = 0;
  nbpixels = 0;
  width = 0;
  height = 0;
  ptsDiff = 0;
}

VideoFrame::VideoFrame( const VideoFrame& tocopy ) : QSharedData( tocopy )
{
    if ( tocopy.frame.octets != NULL )
    {
        nboctets = tocopy.nboctets;
        nbpixels = tocopy.nboctets / Pixel::NbComposantes;
        width = tocopy.width;
        height = tocopy.height;
        ptsDiff = tocopy.ptsDiff;
        frame.octets = new quint8[tocopy.nboctets];

        memcpy( frame.octets, tocopy.frame.octets, nboctets );
    }
    else
    {
        nboctets = 0;
        nbpixels = 0;
        ptsDiff = 0;
        frame.octets = NULL;
    }
}

//
//
//
//
//

LightVideoFrame::LightVideoFrame()
{
  m_videoFrame = new VideoFrame;
}

LightVideoFrame::LightVideoFrame( const LightVideoFrame& tocopy ) : m_videoFrame(tocopy.m_videoFrame)
{
}

LightVideoFrame&
LightVideoFrame::operator=( const LightVideoFrame& tocopy )
{
  m_videoFrame = tocopy.m_videoFrame;
  return *this;
}

LightVideoFrame::LightVideoFrame( quint32 width, quint32 height )
{
  m_videoFrame = new VideoFrame;
  m_videoFrame->width = width;
  m_videoFrame->height = height;
  m_videoFrame->nbpixels = m_videoFrame->width * m_videoFrame->height;
  m_videoFrame->nboctets = m_videoFrame->nbpixels * Pixel::NbComposantes;
  m_videoFrame->frame.octets = new quint8[m_videoFrame->nboctets];
  m_videoFrame->ptsDiff = 0;
}

LightVideoFrame::LightVideoFrame( const quint8 * tocopy, quint32 width, quint32 height )
{
    m_videoFrame = new VideoFrame;
    m_videoFrame->width = width;
    m_videoFrame->height = height;
    m_videoFrame->nbpixels = m_videoFrame->width * m_videoFrame->height;
    m_videoFrame->nboctets = m_videoFrame->nbpixels * Pixel::NbComposantes;
    m_videoFrame->frame.octets = new quint8[m_videoFrame->nboctets];
    m_videoFrame->ptsDiff = 0;

    memcpy( m_videoFrame->frame.octets, tocopy, m_videoFrame->nboctets );
}

LightVideoFrame::~LightVideoFrame()
{
}

const VideoFrame*
LightVideoFrame::operator->( void ) const
{
  return m_videoFrame.data();
}

const VideoFrame&
LightVideoFrame::operator*( void ) const
{
  return *m_videoFrame;
}

VideoFrame*
LightVideoFrame::operator->( void )
{
  return m_videoFrame.data();
}

VideoFrame&
LightVideoFrame::operator*( void )
{
  return *m_videoFrame;
}
