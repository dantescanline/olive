﻿#include "audiomonitor.h"

#include "project/sequence.h"
#include "playback/audio.h"
#include "panels/panels.h"
#include "panels/timeline.h"

#include <QPainter>
#include <QLinearGradient>
#include <QtMath>

#include <QDebug>

#define AUDIO_MONITOR_PEAK_HEIGHT 15
#define AUDIO_MONITOR_GAP 3

extern "C" {
#include "libavformat/avformat.h"
}

AudioMonitor::AudioMonitor(QWidget *parent) :
	QWidget(parent)
{
	values.resize(2);
	values.fill(1.0);
}

void AudioMonitor::set_value(const QVector<double> &ivalues) {
	values = ivalues;
	update();
}

void AudioMonitor::resizeEvent(QResizeEvent *e) {
	gradient = QLinearGradient(QPoint(0, rect().top()), QPoint(0, rect().bottom()));
	gradient.setColorAt(0, Qt::red);
	gradient.setColorAt(0.25, Qt::yellow);
	gradient.setColorAt(1, Qt::green);
	QWidget::resizeEvent(e);
}

void AudioMonitor::paintEvent(QPaintEvent *) {
	if (sequence != nullptr) {
		QPainter p(this);
		int channel_x = AUDIO_MONITOR_GAP;
//		int channel_count = av_get_channel_layout_nb_channels(sequence->audio_layout);
		int channel_count = values.size();
		int channel_width = (width()/channel_count) - AUDIO_MONITOR_GAP;
		int i;
		for (i=0;i<channel_count;i++) {
			QRect r(channel_x, AUDIO_MONITOR_PEAK_HEIGHT + AUDIO_MONITOR_GAP, channel_width, height());
			p.fillRect(r, gradient);

			bool peak = false;

			r.setHeight(qRound(r.height()*(values.at(i))));

			QRect peak_rect(channel_x, 0, channel_width, AUDIO_MONITOR_PEAK_HEIGHT);
			if (peak) {
				p.fillRect(peak_rect, QColor(255, 0, 0));
			} else {
				p.fillRect(peak_rect, QColor(64, 0, 0));
			}

			p.fillRect(r, QColor(0, 0, 0, 160));

			channel_x += channel_width + AUDIO_MONITOR_GAP;
		}
	}
}
