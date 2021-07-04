#include "MainWindow.h"
#include "GUI\process\DialogSelectProcess.h"
#include "GUI\memory\QHexView\document\buffer\qmemorybuffer.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	SetupToolBar();
	SetupHexView();

	connect(&_Timer, &QTimer::timeout, this, &MainWindow::OnTimeTick);
}

void MainWindow::SetupToolBar()
{
	QAction* actOpenProcess = new QAction(QIcon(":/M/resources/Icons/openprocess128x128.png"), tr("&Open..."), this);
	actOpenProcess->setStatusTip("打开进程");
	actOpenProcess->setToolTip("打开进程");
	ui.mainToolBar->addAction(actOpenProcess);
	connect(actOpenProcess, &QAction::triggered, this, &MainWindow::OnActionOpenProcess);

	//QAction* actMemory = new QAction(QIcon(":/MAIN/resources/Icons/edit128x128.png"), tr("&Memory"), this);
	//actMemory->setStatusTip("显示内存视图");
	//actMemory->setToolTip("显示内存视图");
	//ui.mainToolBar->addAction(actMemory);
	//connect(actMemory, &QAction::triggered, this, &MainWindow::OnActionOpenMemory);
}

void MainWindow::SetupHexView()
{
	for (int i = 0; i < 50; i++)
	{
		for (int col = 0; col < 16; col++)
		{
			_ByteArray.append('\x00');
		}
	}

	QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(_ByteArray);
	document->setBaseAddress(0);

	_HexView = new QHexView(this);
	_HexView->setReadOnly(true);
	_HexView->setDocument(document);

	setCentralWidget(_HexView);

	//_Timer.start(1000);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	QMainWindow::closeEvent(event);
}

void MainWindow::OnActionOpenProcess(bool checked)
{
	DialogSelectProcess dialog;

	if (dialog.exec() == QDialog::Rejected)
	{
		return;
	}

	if (!SEngine.AttachSelectedProcess())
	{
		qWarning("关联进程失败");
		QMessageBox::warning(this, "", "关联进程失败");
		return;
	}
}

void MainWindow::OnTimeTick()
{
	static int value = 0x01;
	auto document = _HexView->document();
	document->replace(2, value);

	QHexMetadata* hexmetadata = document->metadata();
	hexmetadata->comment(value, 0, 2, "test");

	value++;
}
