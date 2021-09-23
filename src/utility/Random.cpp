#include "Random.h"
#include <QString>
#include <QRandomGenerator>
#include <QDateTime>

//static QString RANDOM_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-=_+{}[]<>,./?'~`\"";
static QString RANDOM_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890@-+*.";

QByteArray Random::GenerateString(qint32 count, int sp)
{
	auto current = QDateTime::currentMSecsSinceEpoch();
	QRandomGenerator prng(current + sp);
	QByteArray result;

	for (int i = 0; i < count; i++) {
		auto rand_value = prng.generate();
		auto ch = RANDOM_CHARS.at(rand_value % RANDOM_CHARS.length());
		result.append(ch);
	}

	return result;
}
