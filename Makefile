all:
	g++ src/main.cpp src/base64.h src/base64.cpp src/notification/notifier.h src/notification/notifier.cpp src/notification/notificationEmail.h src/notification/notificationNone.h src/notification/SimpleEmailSender.h src/notification/SimpleEmailSender.cpp src/notification/SSLSimpleEmailSender.h src/notification/SSLSimpleEmailSender.cpp src/IConfig/IConfig.cpp src/IConfig/IConfig.h src/IConfig/iniparser.h src/IConfig/iniparser.cpp -o release/result -lssl -lcrypto -lm -std=c++11
	@echo "result move in ./release"
