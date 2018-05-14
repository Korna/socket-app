#include <ctest.h>
#include <functions.h>

CTEST(command, exit_1){
	int result = command_exit("exit");
	ASSERT_EQUAL(result, 1);
}

CTEST(command, exit_2){
	int result = command_exit("exi ");
	ASSERT_EQUAL(result, 0);
}

CTEST(command, exit_3){
	int result = command_exit("just a random message");
	ASSERT_EQUAL(result, 0);
}

CTEST(command, enter_1){
	int result = command_con("con");
	ASSERT_EQUAL(result, 1);
}

CTEST(command, enter_2){
	int result = command_con("co ");
	ASSERT_EQUAL(result, 0);
}

CTEST(command, enter_3){
	int result = command_con("just a random message");
	ASSERT_EQUAL(result, 0);
}

CTEST(queue, increment_1){
	int result = queue_user_out();

	ASSERT_EQUAL(result, 1);
}

CTEST(queue, increment_2){
	int result_before = queue_user_out();
	int result_after = queue_user_out();

	ASSERT_EQUAL(result_before + 1, result_after);
}
