/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This logoFile is part of the examples of the Qt Wayland module
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this logoFile in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this logoFile under the terms of the BSD license
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
#include "MainWidget.h"

#include "QrCode.hpp"

#include <QFile>
#include <QExposeEvent>
#include <QResizeEvent>
#include <QWindow>
#include <QPainter>
#include <QXmlStreamReader>
#include <QSvgRenderer>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QPushButton>

Window::Window()
	:QWidget()
{
}

void Window::drawBackground()
{
    // Get logo
    QFile logoFile(":/umbrel.svg");
    QFile QRLogoFile(":/umbrel-qr-icon.svg");
    logoFile.open(QIODevice::ReadOnly);
    QRLogoFile.open(QIODevice::ReadOnly);
    QString logoString = QTextStream(&logoFile).readAll();
    QString qrLogoString = QTextStream(&QRLogoFile).readAll();

    // Get tor host name
    QFile torHostNamelogoFile("/home/umbrel/umbrel/tor/data/web/hostname");
    torHostNamelogoFile.open(QIODevice::ReadOnly);
    QTextStream hostNameTextStream(&torHostNamelogoFile);
    QString torHostName = "http://" + hostNameTextStream.readAll();

    // Get IP
    QString ipAddress;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            ipAddress = address.toString();
        }
    }

    // Prepare images
    QImage backgroundColorImage = QImage(this->geometry().size(), QImage::Format_RGB32);
    QImage backgroundImage(500, 500, QImage::Format_ARGB32);
    QImage qrCenterImage(136, 136, QImage::Format_ARGB32);
    QImage logoImage(68, 76.5, QImage::Format_ARGB32);
    backgroundColorImage.fill(QColor(BACKGKROUND_COLOR));
    backgroundImage.fill(Qt::white);
    qrCenterImage.fill(Qt::white);
    logoImage.fill(QColor(BACKGKROUND_COLOR));
    QPainter painter(this);
    QPainter QRLogoPainter(&qrCenterImage);
    QSvgRenderer *renderer = getQrCode(&torHostName);
    renderer->render(&painter);
    int deltaX = this->geometry().width() - backgroundImage.width();
    int deltaY = this->geometry().height() - backgroundImage.height() + 260;
    int deltaX2 = backgroundImage.width() - 136;
    int deltaY2 = backgroundImage.height() - 136;
    QXmlStreamReader *reader = new QXmlStreamReader(qrLogoString);
    QSvgRenderer *renderer1 = new QSvgRenderer(reader);
    renderer1->render(&QRLogoPainter);

    // Draw background with QR code
    QPainter textPainter(this);
    textPainter.drawImage(this->geometry(), backgroundColorImage);
    textPainter.translate(deltaX / 2, deltaY / 2);
    textPainter.drawImage(backgroundImage.rect(), backgroundImage);
    textPainter.translate(deltaX2 / 2, deltaY2 / 2);
    textPainter.drawImage(qrCenterImage.rect(), qrCenterImage);
    textPainter.translate(-(deltaX / 2), -(deltaY / 2));
    textPainter.translate(-(deltaX2 / 2), -(deltaY2 / 2));
    textPainter.setPen(QPen(QColor(TEXT_COLOR)));
    textPainter.setFont(QFont(DEFAULT_FONT, 50, QFont::Bold));
    textPainter.translate(0, 20 + deltaY / 2 - 300);
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "Welcome!");
    textPainter.translate(0, 90);
    textPainter.setFont(QFont(DEFAULT_FONT, 20, QFont::Bold));
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "Your Umbrel is up and running at:");
    textPainter.translate(0, 40);
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "http://umbrel.local");
    textPainter.translate(0, 40);
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "http://" + ipAddress);
    textPainter.translate(0, 40);
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, torHostName);
    textPainter.translate(0, 600);
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "Thank you for using Umbrel!");

    // Draw Umbrel logo in the bottom left corner
    QPainter logoPainter(&logoImage);
    QXmlStreamReader *reader1 = new QXmlStreamReader(logoString);
    QSvgRenderer *renderer2 = new QSvgRenderer(reader1);
    renderer2->render(&logoPainter);
    int deltaX3 = this->geometry().width() - logoImage.width() - 15;
    int deltaY3 = this->geometry().height() - logoImage.height() - 15;
    textPainter.translate(deltaX3, -(20 + deltaY / 2 - 300) - 600 - 40 * 3 - 90 + deltaY3);
    textPainter.drawImage(logoImage.rect(), logoImage);
    textPainter.end();
}

void Window::update()
{
    drawBackground();
}
QSvgRenderer *Window::getQrCode(QString *address) {
    qrcodegen::QrCode qr0 = qrcodegen::QrCode::encodeText(address->toStdString().c_str(), qrcodegen::QrCode::Ecc::HIGH);
    QString svg = QString::fromStdString(qr0.toSvgString(4));
    QXmlStreamReader *reader = new QXmlStreamReader(svg);
    QSvgRenderer *renderer = new QSvgRenderer(reader);
    return renderer;
}


void Window::handleButton() {
    layout()->removeItem(layout()->itemAt(0));
    layout()->removeItem(layout()->itemAt(0));
    layout()->removeItem(layout()->itemAt(0));
    QWebEngineView *view = new QWebEngineView(this);
    view->setUrl(QUrl("http://localhost"));
    view->show();
    layout()->addWidget(view);
}

void Window::init() {
    QVBoxLayout *layout = new QVBoxLayout;
    MainWidget *mainWidget = new MainWidget();
    layout->setSpacing(0);
    layout->setMargin(0);
    QPushButton *button = new QPushButton("Open the Umbrel dashboard");
    button->setMinimumHeight(75);
    layout->addWidget(mainWidget);
    layout->addWidget(button);
    setLayout(layout);
    connect(button, &QPushButton::clicked, this, &Window::handleButton);
}
