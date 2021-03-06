#include "unity.h"
#include "command_prompt.h"
#include "history_buffer.h"
#include <stdio.h>
#include <malloc.h>
#include "mock_get_ch.h"
#include "mock_putch.h"


void setUp(void)
{
	cursor = 0;									
	next_status = 0;
	previous_status = 0;
	arrow_left_right_home_insert_status = 0;
	end_of_program = 0;
	isInsert = 0;
	isEnter = 0;
}

void tearDown(void)
{
}



/* To test get_key_press
 * when press 1 will return 49 because 49 is the ascii code for number 1
 */
void test_get_key_press_given_1_should_return_49()
{
	int return_value;  
		
	//mock
	get_character_ExpectAndReturn('1');
	
	//run
	return_value = get_key_press();
	TEST_ASSERT_EQUAL(49, return_value);
}



//test for the special key without escape code
void test_IsSpecialKey_given_escape_key_should_return_CODE_ESCAPE()
{
	int return_value;  		// to get the return from get_key_press function
	int status;				// to get the return from IsSpecialKey function
		
	//mock
	get_character_ExpectAndReturn(KEY_ESCAPE);
	
	//run
	return_value = get_key_press();
	status = IsSpecialKey(return_value);
	TEST_ASSERT_EQUAL( CODE_ESCAPE , status);
}



//test for the special key with escape code
void test_IsSpecialKey_given_arrow_up_should_return_CODE_ARROWUP()
{
	int return_value;  		// to get the return from get_key_press function
	int status;				// to get the return from IsSpecialKey function
		
	//mock
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	
	//run
	return_value = get_key_press();
	status = IsSpecialKey(return_value);
	TEST_ASSERT_EQUAL( CODE_ARROWUP , status);
}



/* test for non special key
 * b is not a special key so will return 0
 */
void test_IsSpecialKey_given_b_should_return_0()
{
	int return_value;  		// to get the return from get_key_press function
	int status;				// to get the return from IsSpecialKey function
		
	//mock
	get_character_ExpectAndReturn('b');
	
	//run
	return_value = get_key_press();
	status = IsSpecialKey(return_value);
	TEST_ASSERT_EQUAL( 0 , status);
}



//when special key is pressed, it will jump out from getting input from user, hence buffer stop at the last input
void test_user_input_interface_given_abc_and_enter_key_buffer_should_get_abc()
{
	int key_return;  // the variable which will get the code which consists of escape and ascii code
	
	// mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);
	
	// run
	key_return = user_input_interface();
	TEST_ASSERT_EQUAL_STRING("abc", user_input);
	TEST_ASSERT_EQUAL( CODE_ENTER, key_return); 
}




void test_movecursortoend_given_user_input_abcd_should_get_cursor_is_4()
{
	char string[] = "abcd";
	
	cursor = movecursortoend(string);
	TEST_ASSERT_EQUAL(4, cursor);

}




// to test the handle backspace function
void test_handle_BACKSPACE_given_abcde_should_get_abcd()
{

	// mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn(KEY_BACKSPACE); 
	
	// run
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abcd", user_input);
	TEST_ASSERT_EQUAL(4, cursor);
}




// to test the handle backspace function for twice
void test_handle_BACKSPACE_twice_given_abcde_should_get_abc()
{
	
	// mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn(KEY_BACKSPACE);	
	get_character_ExpectAndReturn(KEY_BACKSPACE);

	// run
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abc", user_input);
	TEST_ASSERT_EQUAL(3, cursor);
}



// to test the handle backspace function when no more char cursor should stay
void test_handle_BACKSPACE_given_1plus2_when_backspace_four_times_should_get_cursor_is_0()
{
	
	// mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('+');
	put_character_Expect('+');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn(KEY_BACKSPACE);	
	get_character_ExpectAndReturn(KEY_BACKSPACE);
	get_character_ExpectAndReturn(KEY_BACKSPACE);
	get_character_ExpectAndReturn(KEY_BACKSPACE);

	// run
	main_command_prompt();
	TEST_ASSERT_EQUAL(2, cursor);
	TEST_ASSERT_EQUAL_STRING("1+", user_input);
	main_command_prompt();
	TEST_ASSERT_EQUAL(1, cursor);
	TEST_ASSERT_EQUAL_STRING("1", user_input);
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("", user_input);
	TEST_ASSERT_EQUAL(0, cursor);
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("", user_input);
	TEST_ASSERT_EQUAL(0, cursor);
}




// to test the handle backspace function given abc backspace once then enter char d should get abd
void test_handle_BACKSPACE_given_abc_backspace_once_then_enter_d_should_get_abd()
{
	
	// mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_BACKSPACE);	
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);	// the key entered here is not important, just wanted to get out from the loop
	
	// run
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("ab", user_input);
	TEST_ASSERT_EQUAL(2, cursor);
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abd", user_input);
	TEST_ASSERT_EQUAL(2, cursor);	//should be point at 3 but because of arrow left so point at 2

}



/* to test the handle backspace function given string abc 
 * Cursor point at b
 * backspace is pressed
 * Expect:
 *			user_input to be bc
 */
void test_handle_BACKSPACE_given_abc_cursor_at_b_backspace_should_get_bc()
{
	
	// mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(KEY_BACKSPACE);

	
	// run
	main_command_prompt();
	TEST_ASSERT_EQUAL(2, cursor);
	main_command_prompt();
	TEST_ASSERT_EQUAL(1, cursor);
	main_command_prompt();
	TEST_ASSERT_EQUAL(0, cursor);
	TEST_ASSERT_EQUAL('b', user_input[cursor]);
	TEST_ASSERT_EQUAL_STRING("bc", user_input);

}



void test_initialize_historybuffer_given_length_of_buffer_is_5_should_allocate_length_of_5_to_the_buffer()
{
	initialize_historybuffer(5);
	TEST_ASSERT_EQUAL(5, hb->length);
}



//when user type string of abc and press enter, abc should go into buffer[0]
void test_given_abc_when_handle_ENTER_is_called_should_go_into_buffer()
{
	initialize_historybuffer(3);			//initialize history buffer
	
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);

	//run
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abc", hb->buffer[0]);
}



/* Given strings of abc  1+2  3-4  and each of it are insert into history buffer
 * when enter is press
 * Expect : buffer[0]	=	abc
 * 			buffer[1]	=	1+2
 *			buffer[2]	=	3-4
 */
void test_given_abc_1plus2_3minus4_when_handle_ENTER_is_called_should_go_into_buffer_respectively()
{
	initialize_historybuffer(3);			//initialize history buffer
	
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('+');
	put_character_Expect('+');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn('-');
	put_character_Expect('-');
	get_character_ExpectAndReturn('4');
	put_character_Expect('4');
	get_character_ExpectAndReturn(KEY_ENTER);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abc", hb->buffer[0]);
	TEST_ASSERT_EQUAL_STRING("1+2", hb->buffer[1]);
	TEST_ASSERT_EQUAL_STRING("3-4", hb->buffer[2]);
}	



//boundary test for handle_ENTER
void test_handle_ENTER_given_empty_user_input_should_not_add_into_buffer()
{
	initialize_historybuffer(3);			//initialize history buffer
	
	//mock
	get_character_ExpectAndReturn(KEY_ENTER);


	//run
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING(NULL, hb->buffer[0]);
}




/* Given strings of abc  def  ghi  are already inside history buffer
 * Now arrow up key is pressed.
 * Expect:
 *			ghi		return and stored into user_input
 * 			
 */
void test_handle_ARROWUP_given_abc_def_ghi_are_inside_history_buffer_when_arrow_up_is_pressed_ghi_should_return_to_user_input()
{
	initialize_historybuffer(3);			//initialize history buffer
	
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn('f');
	put_character_Expect('f');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('g');
	put_character_Expect('g');
	get_character_ExpectAndReturn('h');
	put_character_Expect('h');
	get_character_ExpectAndReturn('i');
	put_character_Expect('i');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("ghi", user_input);
}




/* Given strings of abc  def  ghi  are already inside history buffer
 * Now arrow up key is pressed twice
 * Expect:
 *			def	return and stored into user_input
 * 			
 */
void test_handle_ARROWUP_given_abc_def_ghi_are_inside_history_buffer_when_arrow_up_is_pressed_twice_def_should_return_to_user_input()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn('f');
	put_character_Expect('f');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('g');
	put_character_Expect('g');
	get_character_ExpectAndReturn('h');
	put_character_Expect('h');
	get_character_ExpectAndReturn('i');
	put_character_Expect('i');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("def", user_input);
}



/* Test when already reached the oldest input should return the same input
 * Given strings of abc  def  ghi  are already inside history buffer
 * Now arrow up key is pressed four times
 * Expect:
 *			abc is returned
 * 			
 */
void test_given_abc_def_ghi_are_inside_history_buffer_when_arrow_up_is_pressed_four_times_should_throw_error()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn('f');
	put_character_Expect('f');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('g');
	put_character_Expect('g');
	get_character_ExpectAndReturn('h');
	put_character_Expect('h');
	get_character_ExpectAndReturn('i');
	put_character_Expect('i');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("ghi", user_input);
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("def", user_input);
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abc", user_input);
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abc", user_input);
}




/* Given strings of abc  def  ghi  are already inside history buffer
 * Now arrow up key is pressed twice
 * Then arrow down is pressed once
 * Expect:
 *			ghi return and store in user_input
 * 			
 */
void test_handle_ARROWDOWN_given_abc_def_ghi_are_inside_history_buffer_when_arrow_up_is_pressed_twice_arrow_down_press_once_ghi_should_store_in_user_input()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn('f');
	put_character_Expect('f');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('g');
	put_character_Expect('g');
	get_character_ExpectAndReturn('h');
	put_character_Expect('h');
	get_character_ExpectAndReturn('i');
	put_character_Expect('i');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_DOWN);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("ghi", user_input);
}





/* Given strings of abc  def  ghi  are already inside history buffer
 * Type ggwp but no enter first
 * Now arrow up key is pressed once
 * Then arrow down is pressed once
 * Expect:
 *			user_input to be ggwp
 * 			
 */
void test_handle_ARROWUP_and_handle_ARROWUP()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn('f');
	put_character_Expect('f');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('g');
	put_character_Expect('g');
	get_character_ExpectAndReturn('h');
	put_character_Expect('h');
	get_character_ExpectAndReturn('i');
	put_character_Expect('i');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('g');
	put_character_Expect('g');
	get_character_ExpectAndReturn('g');
	put_character_Expect('g');
	get_character_ExpectAndReturn('w');
	put_character_Expect('w');
	get_character_ExpectAndReturn('p');
	put_character_Expect('p');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_UP);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_DOWN);

	
	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("ggwp", user_input);
}




void test_handle_PAGEUP_given_123_456_789_000_when_pageup_is_pressed_should_get_123()
{
	initialize_historybuffer(5);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('4');
	put_character_Expect('4');
	get_character_ExpectAndReturn('5');
	put_character_Expect('5');
	get_character_ExpectAndReturn('6');
	put_character_Expect('6');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('7');
	put_character_Expect('7');
	get_character_ExpectAndReturn('8');
	put_character_Expect('8');
	get_character_ExpectAndReturn('9');
	put_character_Expect('9');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('0');
	put_character_Expect('0');
	get_character_ExpectAndReturn('0');
	put_character_Expect('0');
	get_character_ExpectAndReturn('0');
	put_character_Expect('0');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_PAGEUP);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("123", user_input);
}




void test_handle_PAGEDOWN_given_123_456_789_000_when_pagedown_is_pressed_should_get_000()
{
	initialize_historybuffer(5);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('4');
	put_character_Expect('4');
	get_character_ExpectAndReturn('5');
	put_character_Expect('5');
	get_character_ExpectAndReturn('6');
	put_character_Expect('6');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('7');
	put_character_Expect('7');
	get_character_ExpectAndReturn('8');
	put_character_Expect('8');
	get_character_ExpectAndReturn('9');
	put_character_Expect('9');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn('0');
	put_character_Expect('0');
	get_character_ExpectAndReturn('0');
	put_character_Expect('0');
	get_character_ExpectAndReturn('0');
	put_character_Expect('0');
	get_character_ExpectAndReturn(KEY_ENTER);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_PAGEDOWN);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("000", user_input);
}




void test_handle_arrow_left_given_123_call_once_should_point_at_3()
{
	initialize_historybuffer(5);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);

	//run
	main_command_prompt();
	TEST_ASSERT_EQUAL(2 , cursor);
	TEST_ASSERT_EQUAL('3', user_input[cursor]);
}




void test_handle_arrow_left_given_123_call_twice_should_point_at_2()
{
	initialize_historybuffer(5);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);

	//run
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(1 , cursor);
	TEST_ASSERT_EQUAL('2', user_input[cursor]);
}



void test_handle_arrow_left_given_123_call_twice_and_enter_char_d_should_get_1d3()
{
	initialize_historybuffer(5);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_RIGHT);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(3 , cursor);
	TEST_ASSERT_EQUAL_STRING("1d3", user_input);
}



// boundary test for handle_ARROWLEFT
// The cursor should stay when it is already at first character
void test_handle_arrow_left_given_123_call_thrice_should_point_at_1()
{
	initialize_historybuffer(5);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	
	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(0 , cursor);
	TEST_ASSERT_EQUAL('1', user_input[cursor]);
}



void test_handle_arrow_right_given_string_of_123_cursor_pointed_at_1_call_once_should_point_at_2()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_RIGHT);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(1 , cursor);
	TEST_ASSERT_EQUAL('2', user_input[cursor]);
}



// boundary test for handle_ARROWRIGHT
void test_handle_arrow_right_given_string_of_123_cursor_pointed_at_3_call_twice_should_stay_the_same()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_RIGHT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_RIGHT);

	//run
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(3 , cursor);
	TEST_ASSERT_EQUAL('\0', user_input[cursor]);
	main_command_prompt();
	TEST_ASSERT_EQUAL(3 , cursor);
	TEST_ASSERT_EQUAL('\0', user_input[cursor]);
}




void test_handle_HOME_given_123_cursor_is_at_2_when_home_is_press_should_get_cursor_is_0()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_HOME);

	//run
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(1 , cursor);
	TEST_ASSERT_EQUAL('2', user_input[cursor]);
	main_command_prompt();
	TEST_ASSERT_EQUAL(0 , cursor);
	TEST_ASSERT_EQUAL('1', user_input[cursor]);
}




void test_handle_END_given_123_cursor_is_at_2_when_END_is_press_should_get_cursor_is_3()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('1');
	put_character_Expect('1');
	get_character_ExpectAndReturn('2');
	put_character_Expect('2');
	get_character_ExpectAndReturn('3');
	put_character_Expect('3');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_END);

	//run
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(1 , cursor);
	TEST_ASSERT_EQUAL('2', user_input[cursor]);
	main_command_prompt();
	TEST_ASSERT_EQUAL(3 , cursor);
	TEST_ASSERT_EQUAL('\0', user_input[cursor]);
}




void test_handle_DEL_given_abc_cursor_at_c_del_is_press_should_get_ab()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_DELETE);

	//run
	main_command_prompt();
	TEST_ASSERT_EQUAL(2 , cursor);
	TEST_ASSERT_EQUAL('c', user_input[cursor]);
	main_command_prompt();
	TEST_ASSERT_EQUAL(2 , cursor);
	TEST_ASSERT_EQUAL('\0', user_input[cursor]);
	TEST_ASSERT_EQUAL_STRING("ab", user_input);
}



//boundary test for delete
void test_handle_DEL_given_abc_cursor_at_behind_c_del_is_press_should_get_abc()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_DELETE);

	//run
	main_command_prompt();
	TEST_ASSERT_EQUAL(3 , cursor);
	TEST_ASSERT_EQUAL('\0', user_input[cursor]);
	TEST_ASSERT_EQUAL_STRING("abc", user_input);
}



void test_handle_DEL_given_abcde_cursor_at_b_del_is_press_should_get_acde()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_DELETE);

	//run
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(1 , cursor);
	TEST_ASSERT_EQUAL('b', user_input[cursor]);
	main_command_prompt();
	TEST_ASSERT_EQUAL(1 , cursor);
	TEST_ASSERT_EQUAL('c', user_input[cursor]);
	TEST_ASSERT_EQUAL_STRING("acde", user_input);
}



void test_handle_INSERT_given_isInsert_should_toggle_when_called()
{

	isInsert = 0;
	handle_INSERT();
	TEST_ASSERT_EQUAL(1, isInsert);
	handle_INSERT();
	TEST_ASSERT_EQUAL(0, isInsert);
	handle_INSERT();
	TEST_ASSERT_EQUAL(1, isInsert);
	handle_INSERT();
	TEST_ASSERT_EQUAL(0, isInsert);
}




void test_movecharactersbackward_given_abcdef_cursor_pointed_at_c_should_get_abccdef()
{
	char buffer[20] = "abcdef";
	cursor = 2;
	
	int endofinput = movecursortoend(buffer);
	TEST_ASSERT_EQUAL(6,endofinput);
	movecharactersbackward(endofinput, buffer);
	TEST_ASSERT_EQUAL_STRING("abccdef",buffer);
}




void test_handle_INSERT_given_abc_cursor_at_b_enter_xyz_should_get_axyzbc()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_INSERT);
	get_character_ExpectAndReturn('x');
	get_character_ExpectAndReturn('y');
	get_character_ExpectAndReturn('z');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);	//this key is just to get out from the program loop

	
	//run
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(1 , cursor);
	TEST_ASSERT_EQUAL('b', user_input[cursor]);
	main_command_prompt();
	TEST_ASSERT_EQUAL_STRING("abc", user_input);
	TEST_ASSERT_EQUAL(1, isInsert);
	TEST_ASSERT_EQUAL(1 , arrow_left_right_home_insert_status);
	main_command_prompt();
	TEST_ASSERT_EQUAL(3 , cursor); 	//this is 3 instead of 4 because of the arrow left
	TEST_ASSERT_EQUAL('z', user_input[cursor]);
	TEST_ASSERT_EQUAL_STRING("axyzbc", user_input);
	
}




//boundary test for INSERT
void test_handle_INSERT_given_abcdef_cursor_at_behind_f_enter_z_should_get_abcdefz()
{
	initialize_historybuffer(3);			//initialize history buffer
		
	//mock
	get_character_ExpectAndReturn('a');
	put_character_Expect('a');
	get_character_ExpectAndReturn('b');
	put_character_Expect('b');
	get_character_ExpectAndReturn('c');
	put_character_Expect('c');
	get_character_ExpectAndReturn('d');
	put_character_Expect('d');
	get_character_ExpectAndReturn('e');
	put_character_Expect('e');
	get_character_ExpectAndReturn('f');
	put_character_Expect('f');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(KEY_INSERT);
	get_character_ExpectAndReturn('z');
	get_character_ExpectAndReturn(ESCAPECODE2);
	get_character_ExpectAndReturn(ARROW_LEFT);	//this key is just to get out from the program loop
	

	//run
	main_command_prompt();
	main_command_prompt();
	TEST_ASSERT_EQUAL(6 , cursor); 
	TEST_ASSERT_EQUAL('z', user_input[cursor]);
	TEST_ASSERT_EQUAL_STRING("abcdefz", user_input);
}


