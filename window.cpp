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

#include "config.h"
#include "window.h"

#include <QFile>
#include <QExposeEvent>
#include <QResizeEvent>
#include <QWindow>
#include <QBackingStore>
#include <QPainter>
#include <QXmlStreamReader>
#include <QSvgRenderer>
#include <QHostAddress>
#include <QNetworkInterface>

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
    // Get logo
    QFile file(":/umbrel.svg");
    QFile file1(":/umbrel2.svg");
    file.open(QIODevice::ReadOnly);
    file1.open(QIODevice::ReadOnly);
    QTextStream s(&file);
    QTextStream s1(&file1);
    QString logoString = s.readAll();
    QString logoString1 = s1.readAll();

    // Get tor host name
    QFile file2("/home/umbrel/umbrel/tor/data/web/hostname");
    file2.open(QIODevice::ReadOnly);
    QTextStream s2(&file2);
    QString torAddress = "http://" + s2.readAll();

    // Get IP
    QString ipAddress;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            ipAddress = address.toString();
        }
    }

    // Prepare images
    QPaintDevice *device = m_backingStore->paintDevice();
    QImage backgroundColorImage = QImage(this->geometry().size(), QImage::Format_RGB32);
    QImage backgroundImage(500, 500, QImage::Format_ARGB32);
    QImage logoImage(136, 153, QImage::Format_ARGB32);
    QImage logoImage1(68, 76.5, QImage::Format_ARGB32);
    backgroundColorImage.fill(QColor(BACKGKROUND_COLOR));
    backgroundImage.fill(Qt::white);
    logoImage.fill(Qt::white);
    logoImage1.fill(QColor(BACKGKROUND_COLOR));
    QPainter painter(&backgroundImage);
    QPainter painter2(&logoImage);
    QSvgRenderer *renderer = getQrCode(&torAddress);
    renderer->render(&painter);
    int deltaX = this->geometry().width() - backgroundImage.width();
    int deltaY = this->geometry().height() - backgroundImage.height() + 260;
    int deltaX2 = backgroundImage.width() - 136;
    int deltaY2 = backgroundImage.height() - 153;
    QXmlStreamReader *reader = new QXmlStreamReader(logoString1);
    QSvgRenderer *renderer1 = new QSvgRenderer(reader);
    renderer1->render(&painter2);

    // Draw background with QR code
    QPainter painter3(device);
    painter3.drawImage(this->geometry(), backgroundColorImage);
    painter3.translate(deltaX / 2, deltaY / 2);
    painter3.drawImage(backgroundImage.rect(), backgroundImage);
    painter3.translate(deltaX2 / 2, deltaY2 / 2);
    painter3.drawImage(logoImage.rect(), logoImage);
    painter3.translate(-(deltaX / 2), -(deltaY / 2));
    painter3.translate(-(deltaX2 / 2), -(deltaY2 / 2));
    painter3.setPen(QPen(QColor(TEXT_COLOR)));
    painter3.setFont(QFont(DEFAULT_FONT, 50, QFont::Bold));
    painter3.translate(0, 20 + deltaY / 2 - 300);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "Welcome!");
    painter3.translate(0, 90);
    painter3.setFont(QFont(DEFAULT_FONT, 20, QFont::Bold));
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "Your Umbrel is up and running at:");
    painter3.translate(0, 40);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "http://umbrel.local");
    painter3.translate(0, 40);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "http://" + ipAddress);
    painter3.translate(0, 40);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, torAddress);
    painter3.translate(0, 600);
    painter3.drawText(this->geometry(), Qt::AlignHCenter, "Thank you for using Umbrel!");

    // Draw Umbrel logo in the bottom left corner
    QPainter painter4(&logoImage1);
    QXmlStreamReader *reader1 = new QXmlStreamReader(logoString);
    QSvgRenderer *renderer2 = new QSvgRenderer(reader1);
    renderer2->render(&painter4);
    int deltaX3 = this->geometry().width() - logoImage1.width() - 15;
    int deltaY3 = this->geometry().height() - logoImage1.height() - 15;
    painter3.translate(deltaX3, -(20 + deltaY / 2 - 300) - 600 - 40 * 3 - 90 + deltaY3);
    painter3.drawImage(logoImage1.rect(), logoImage1);
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
    qrcodegen::QrCode qr0 = qrcodegen::QrCode::encodeText(address->toStdString().c_str(), qrcodegen::QrCode::Ecc::HIGH);
    QString svg = QString::fromStdString(qr0.toSvgString(4));
    QXmlStreamReader *reader = new QXmlStreamReader(svg);
    QSvgRenderer *renderer = new QSvgRenderer(reader);
    return renderer;
}
