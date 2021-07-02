#pragma once

// 解决Qt常亮中文乱码
#define CC(ctx) QStringLiteral(ctx).toUtf8().constData()
#define CV(ctx) QString::fromLocal8Bit(ctx).toUtf8().data()

#define QUOTE(TXT) QString("'%1'").arg(TXT)
#define QUOTE_S(TXT) QString("(%1)").arg(TXT)
#define QUOTE_M(TXT) QString("[%1]").arg(TXT)
#define QUOTE_L(TXT) QString("{%1}").arg(TXT)