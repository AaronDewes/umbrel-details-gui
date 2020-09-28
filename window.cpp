/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Wayland module
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "window.h"

#include <QMouseEvent>
#include <QWindow>
#include <QBackingStore>
#include <QMatrix4x4>
#include <QPainter>
#include <QXmlStreamReader>
#include <QSvgRenderer>
#include <QHostAddress>
#include <QNetworkInterface>

#include <QtWaylandCompositor/qwaylandseat.h>

#include "QrCode.hpp"

Window::Window()
	:QWindow()
	, m_backingStore(new QBackingStore(this))
{
}

void Window::init()
{
    drawBackground();
}

void Window::drawBackground()
{
    // Get tor host name
    QFile file("/home/umbrel/umbrel/tor/data/web/hostname");
    file.open(QIODevice::ReadOnly);
    QTextStream s(&file);
    QString torAddress = "http://" + s.readAll();

    // Get IP
    QString ipAddress;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            ipAddress = address.toString();
        }
    }

    // Draw background with QR code
    QImage backgroundColorImage = QImage(this->geometry().size(), QImage::Format_RGB32);
    backgroundColorImage.fill(QColor(83, 81, 251));
    QImage backgroundImage(500, 500, QImage::Format_ARGB32);
    backgroundImage.fill(Qt::white);
    QPainter painter(&backgroundImage);
    QSvgRenderer *renderer = getQrCode(&torAddress);
    renderer->render(&painter);
    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter2(device);
    int deltaX = this->geometry().width() - backgroundImage.width();
    int deltaY = this->geometry().height() - backgroundImage.height() + 260;
    painter2.drawImage(this->geometry(), backgroundColorImage);
    painter2.translate(deltaX / 2, deltaY / 2);
    painter2.drawImage(backgroundImage.rect(), backgroundImage);
    painter2.end();

    // Add text
    QPainter painter3(device);
    painter3.translate(0, 20 + deltaY / 2 - 300);
    painter3.setPen(QPen(Qt::white));
    painter3.setFont(QFont("Roboto", 50, QFont::Bold));
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "Welcome!");
    painter3.translate(0, 80);
    painter3.setFont(QFont("Roboto", 20, QFont::Bold));
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "Your Umbrel is up and running at:");
    painter3.translate(0, 40);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "http://umbrel.local");
    painter3.translate(0, 40);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "http://" + ipAddress);
    painter3.translate(0, 40);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, torAddress);
    painter3.end();
}
void Window::update()
{
    if (!isExposed())
        return;

    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    drawBackground();
    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

bool Window::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        update();
        return true;
    }
    return QWindow::event(event);
}

void Window::exposeEvent(QExposeEvent *)
{
    if (isExposed())
        update();
}

void Window::resizeEvent(QResizeEvent *resizeEvent)
{
    m_backingStore->resize(resizeEvent->size());
}

QSvgRenderer *Window::getQrCode(QString *address) {
    qrcodegen::QrCode qr0 = qrcodegen::QrCode::encodeText(address->toStdString().c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
    QString svg = QString::fromStdString(qr0.toSvgString(4));
    QXmlStreamReader *reader = new QXmlStreamReader(svg);
    QSvgRenderer *renderer = new QSvgRenderer(reader);
    return renderer;
}
