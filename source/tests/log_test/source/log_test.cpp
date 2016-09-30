/*
 *	Abstract Data Type Library by Parra Studios
 *	Copyright (C) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A abstract data type library providing generic containers.
 *
 */

#include <gmock/gmock.h>

#include <log/log.h>
#include <log/log_level.h>
#include <log/log_handle.h>
#include <log/log_map.h>

class log_test : public testing::Test
{
  public:
};

TEST_F(log_test, DefaultConstructor)
{
	struct log_name_list_type
	{
		const char * name;
		unsigned int value;
	} log_name_list[] =
	{
		{ "test_log_text_sync_seq_stdout",		0 },
		{ "test_log_text_sync_seq_stderr",		1 },
		{ "test_log_text_sync_seq_file",		2 },
		{ "test_log_text_sync_seq_syslog",		3 },
		{ "test_log_text_sync_seq_socket",		4 },
		{ "test_log_bin_async_batch_stdout",	5 },
		{ "test_log_bin_async_batch_stderr",	6 },
		{ "test_log_bin_async_batch_file",		7 },
		{ "test_log_bin_async_batch_syslog",	8 },
		{ "test_log_bin_async_batch_socket",	9 }
	};

	const size_t log_name_list_size = sizeof(log_name_list) / sizeof(log_name_list[0]);

	/* Log map */
	{
		size_t iterator;

		log_map_iterator map_iterator;

		log_map map = log_map_create((size_t)0x00000200);

		EXPECT_NE((log_map) NULL, (log_map) map);

		for (iterator = 0; iterator < log_name_list_size; ++iterator)
		{
			EXPECT_EQ((int) 0, (int) log_map_insert(map, log_name_list[iterator].name, (const void *)&log_name_list[iterator].value));
		}

		EXPECT_EQ((size_t) log_name_list_size, (size_t) log_map_size(map));
		
		for (iterator = 0; iterator < log_name_list_size; ++iterator)
		{
			const void * value_ptr = log_map_get(map, log_name_list[iterator].name);

			unsigned int value = *((unsigned int *)value_ptr);

			EXPECT_EQ((unsigned int) log_name_list[iterator].value, (unsigned int) value);
		}

		for (map_iterator = log_map_iterator_begin(map); log_map_iterator_end(map_iterator) != 0; log_map_iterator_next(map_iterator))
		{
			const char * key = log_map_iterator_key(map_iterator);

			const void * value_ptr = log_map_iterator_value(map_iterator);

			unsigned int value = *((unsigned int *)value_ptr);

			EXPECT_EQ((int) 0, (int) strcmp(log_name_list[value].name, key));
		}

		EXPECT_EQ((int) log_map_destroy(map), (int) 0);
	}

	/* Create logs */
	{
		size_t iterator;

		for (iterator = 0; iterator < log_name_list_size; ++iterator)
		{
			EXPECT_EQ((int) 0, (int) log_create(log_name_list[iterator].name));
		}
	}

	/* Set policies */
	{
		const size_t storage_batch_size = ((size_t)0x00000010);

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[0].name, log_policy_format_text()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[0].name, log_policy_schedule_sync()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[0].name, log_policy_storage_sequential()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[0].name, log_policy_stream_stdio(stdout)));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[1].name, log_policy_format_text()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[1].name, log_policy_schedule_sync()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[1].name, log_policy_storage_sequential()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[1].name, log_policy_stream_stdio(stderr)));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[2].name, log_policy_format_text()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[2].name, log_policy_schedule_sync()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[2].name, log_policy_storage_sequential()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[2].name, log_policy_stream_file(log_name_list[2].name, "a+")));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[3].name, log_policy_format_text()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[3].name, log_policy_schedule_sync()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[3].name, log_policy_storage_sequential()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[3].name, log_policy_stream_syslog(log_name_list[3].name)));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[4].name, log_policy_format_text()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[4].name, log_policy_schedule_sync()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[4].name, log_policy_storage_sequential()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[4].name, log_policy_stream_socket("127.0.0.1", UINT16_C(0x0208))));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[5].name, log_policy_format_binary()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[5].name, log_policy_schedule_async()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[5].name, log_policy_storage_batch(storage_batch_size)));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[5].name, log_policy_stream_stdio(stdout)));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[6].name, log_policy_format_binary()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[6].name, log_policy_schedule_async()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[6].name, log_policy_storage_batch(storage_batch_size)));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[6].name, log_policy_stream_stdio(stderr)));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[7].name, log_policy_format_binary()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[7].name, log_policy_schedule_async()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[7].name, log_policy_storage_batch(storage_batch_size)));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[7].name, log_policy_stream_file(log_name_list[2].name, "a+")));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[8].name, log_policy_format_binary()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[8].name, log_policy_schedule_async()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[8].name, log_policy_storage_batch(storage_batch_size)));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[8].name, log_policy_stream_syslog(log_name_list[3].name)));

		EXPECT_EQ((int) 0, (int) log_define(log_name_list[9].name, log_policy_format_binary()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[9].name, log_policy_schedule_async()));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[9].name, log_policy_storage_batch(storage_batch_size)));
		EXPECT_EQ((int) 0, (int) log_define(log_name_list[9].name, log_policy_stream_socket("127.0.0.1", UINT16_C(0x0209))));
	}

	/* Write simple logs */
	{
		size_t iterator;

		for (iterator = 0; iterator < log_name_list_size; ++iterator)
		{
			EXPECT_EQ((int) 0, (int) log_write(log_name_list[iterator].name, "test_simple_message_tag", LOG_LEVEL_INFO, "hello world"));
		}
	}

	/* Write varidic logs */
	{
		/*
		size_t iterator;

		for (iterator = 0; iterator < log_name_list_size; ++iterator)
		{
			EXPECT_EQ((int) 0, (int) log_write_va(log_name_list[iterator].name, "test_varidic_tag", LOG_LEVEL_INFO, "hello world from log (id : %ul)", iterator));
		}
		*/
	}

	/* Clear all logs */
	{
		size_t iterator;

		for (iterator = 0; iterator < log_name_list_size; ++iterator)
		{
			EXPECT_EQ((int) 0, (int) log_clear(log_name_list[iterator].name));
		}
	}
}