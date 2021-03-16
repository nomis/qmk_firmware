/* Copyright 2021 Simon Arlott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtest/gtest.h"

#include "debounce_test_common.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

extern "C" {
#include "quantum.h"
#include "timer.h"
#include "debounce.h"

void set_time(uint32_t t);
void advance_time(uint32_t ms);
}

void DebounceTest::SetUp() {
    debounce_init(MATRIX_ROWS);
}

void DebounceTest::addEvents(std::initializer_list<DebounceTestEvent> events) {
    events_.insert(events_.end(), events.begin(), events.end());
}

void DebounceTest::runEvents() {
    /* Run the test multiple times, from 1kHz to 10kHz scan rate */
    for (int extra_iterations = 0; extra_iterations < 10; extra_iterations++) {
        if (time_jumps_) {
            /* Don't advance time smoothly, jump to the next event (some tests require this) */
            runEventsInternal(extra_iterations, false);
        } else {
            /* Run the test with both smooth and irregular time; it must produce the same result */
            runEventsInternal(extra_iterations, true);
            runEventsInternal(extra_iterations, false);
        }
    }
}

void DebounceTest::runEventsInternal(int extra_iterations, bool auto_advance_time) {
    matrix_row_t raw_matrix[MATRIX_ROWS];
    matrix_row_t cooked_matrix[MATRIX_ROWS];
    fast_timer_t previous = 0;
    bool first = true;

    /* Initialise keyboard with start time (offset to avoid testing at 0) and all keys UP */
    SetUp();
    set_time(time_offset_);
    std::fill(std::begin(input_matrix_), std::end(input_matrix_), 0);
    std::fill(std::begin(output_matrix_), std::end(output_matrix_), 0);

    for (auto &event : events_) {
        if (!auto_advance_time) {
            /* Jump to the next event */
            set_time(time_offset_ + event.time_);
        } else if (!first && event.time_ == previous + 1) {
            /* This event immediately follows the previous one, don't make extra debounce() calls */
            advance_time(1);
        } else {
            /* Fast forward to the time for this event, calling debounce() with no changes */
            ASSERT_LT((time_offset_ + event.time_) - timer_read_fast(), 60000) << "Test tries to advance more than 1 minute of time";

            while (timer_read_fast() != time_offset_ + event.time_) {
                std::copy(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix));
                std::copy(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix));

                debounce(raw_matrix, cooked_matrix, MATRIX_ROWS, false);

                if (!std::equal(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix))) {
                    FAIL() << "Fatal error: debounce() modified raw matrix at " << strTime(extra_iterations, auto_advance_time)
                        << "\ninput_matrix: changed=false\n" << strMatrix(input_matrix_)
                        << "\nraw_matrix:\n" << strMatrix(raw_matrix);
                }

                if (!std::equal(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix))) {
                    FAIL() << "Unexpected event: debounce() modified cooked matrix at " << strTime(extra_iterations, auto_advance_time)
                    << "\ninput_matrix: changed=false\n" << strMatrix(input_matrix_)
                    << "\nexpected_matrix:\n" << strMatrix(output_matrix_)
                    << "\nactual_matrix:\n" << strMatrix(cooked_matrix);
                }

                advance_time(1);
            }
        }

        first = false;
        previous = event.time_;

        /* Prepare input matrix (verify that events only include changes) */
        for (auto &input : event.inputs_) {
            switch (input.direction_) {
            case DOWN:
                ASSERT_EQ(!!(input_matrix_[input.row_] & (1U << input.col_)), false)
                    << "Test input at " << strTime(extra_iterations, auto_advance_time)
                    << " sets key " << input.row_ << "," << input.col_ << " down but it is already down"
                    << "\ninput_matrix:\n" << strMatrix(input_matrix_);
                input_matrix_[input.row_] |= (1U << input.col_);
                break;

            case UP:
                ASSERT_EQ(!!(input_matrix_[input.row_] & (1U << input.col_)), true)
                    << "Test input at " << strTime(extra_iterations, auto_advance_time)
                    << " sets key " << input.row_ << "," << input.col_ << " up but it is already up"
                    << "\ninput_matrix:\n" << strMatrix(input_matrix_);
                input_matrix_[input.row_] &= ~(1U << input.col_);
                break;
            }
        }

        /* Call debounce */
        std::copy(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix));
        std::copy(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix));

        debounce(raw_matrix, cooked_matrix, MATRIX_ROWS, !event.inputs_.empty());

        if (!std::equal(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix))) {
            FAIL() << "Fatal error: debounce() modified raw matrix at " << strTime(extra_iterations, auto_advance_time)
                << "\ninput_matrix: changed=" << !event.inputs_.empty() << "\n" << strMatrix(input_matrix_)
                << "\nraw_matrix:\n" << strMatrix(raw_matrix);
        }

        /* Prepare output matrix (verify that events only include changes) */
        for (auto &output : event.outputs_) {
            switch (output.direction_) {
            case DOWN:
                ASSERT_EQ(!!(output_matrix_[output.row_] & (1U << output.col_)), false)
                    << "Test output at " << strTime(extra_iterations, auto_advance_time)
                    << " sets key " << output.row_ << "," << output.col_ << " down but it is already down"
                    << "\noutput_matrix:\n" << strMatrix(output_matrix_);
                output_matrix_[output.row_] |= (1U << output.col_);
                break;

            case UP:
                ASSERT_EQ(!!(output_matrix_[output.row_] & (1U << output.col_)), true)
                    << "Test output at " << strTime(extra_iterations, auto_advance_time)
                    << " sets key " << output.row_ << "," << output.col_ << " up but it is already up"
                    << "\noutput_matrix:\n" << strMatrix(output_matrix_);
                output_matrix_[output.row_] &= ~(1U << output.col_);
                break;
            }
        }

        /* Check output matrix has expected change events */
        for (auto &output : event.outputs_) {
            switch (output.direction_) {
            case DOWN:
                EXPECT_EQ(!!(cooked_matrix[output.row_] & (1U << output.col_)), true)
                    << "Missing event at " << strTime(extra_iterations, auto_advance_time)
                    << " expected key " << output.row_ << "," << output.col_ << " down"
                    << "\ninput_matrix: changed=" << !event.inputs_.empty() << "\n" << strMatrix(input_matrix_)
                    << "\nexpected_matrix:\n" << strMatrix(output_matrix_)
                    << "\nactual_matrix:\n" << strMatrix(cooked_matrix);
                break;

            case UP:
                EXPECT_EQ(!!(cooked_matrix[output.row_] & (1U << output.col_)), false)
                    << "Missing event at " << strTime(extra_iterations, auto_advance_time)
                    << " expected key " << output.row_ << "," << output.col_ << " up"
                    << "\ninput_matrix: changed=" << !event.inputs_.empty() << "\n" << strMatrix(input_matrix_)
                    << "\nexpected_matrix:\n" << strMatrix(output_matrix_)
                    << "\nactual_matrix:\n" << strMatrix(cooked_matrix);
                break;
            }
        }

        /* Check output matrix has no other changes */
        if (!std::equal(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix))) {
            FAIL() << "Unexpected event: debounce() cooked matrix does not match expected output matrix"
                << " at " << strTime(extra_iterations, auto_advance_time)
                << "\ninput_matrix: changed=" << !event.inputs_.empty() << "\n" << strMatrix(input_matrix_)
                << "\nexpected_matrix:\n" << strMatrix(output_matrix_)
                << "\nactual_matrix:\n" << strMatrix(cooked_matrix);
        }

        /* Perform some extra iterations of the matrix scan with no changes */
        for (int i = 0; i < extra_iterations; i++) {
            std::copy(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix));
            std::copy(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix));

            debounce(raw_matrix, cooked_matrix, MATRIX_ROWS, false);

            if (!std::equal(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix))) {
                FAIL() << "Fatal error: debounce() modified raw matrix at " << strTime(extra_iterations, auto_advance_time)
                    << " extra iteration " << i << " of " << extra_iterations
                    << "\ninput_matrix: changed=false\n" << strMatrix(input_matrix_)
                    << "\nraw_matrix:\n" << strMatrix(raw_matrix);
            }

            if (!std::equal(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix))) {
                FAIL() << "Unexpected event: debounce() modified cooked matrix at " << strTime(extra_iterations, auto_advance_time)
                << " extra iteration " << i << " of " << extra_iterations
                << "\ninput_matrix: changed=false\n" << strMatrix(input_matrix_)
                << "\nexpected_matrix:\n" << strMatrix(output_matrix_)
                << "\nactual_matrix:\n" << strMatrix(cooked_matrix);
            }
        }
    }

    /* Check that no further changes happen for 1 minute */
    for (int i = 0; i < 60000; i++) {
        matrix_row_t raw_matrix[MATRIX_ROWS];
        matrix_row_t cooked_matrix[MATRIX_ROWS];

        std::copy(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix));
        std::copy(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix));

        debounce(raw_matrix, cooked_matrix, MATRIX_ROWS, false);

        if (!std::equal(std::begin(input_matrix_), std::end(input_matrix_), std::begin(raw_matrix))) {
            FAIL() << "Fatal error: debounce() modified raw matrix at " << strTime(extra_iterations, auto_advance_time)
                << "\ninput_matrix: changed=false\n" << strMatrix(input_matrix_)
                << "\nraw_matrix:\n" << strMatrix(raw_matrix);
        }

        if (!std::equal(std::begin(output_matrix_), std::end(output_matrix_), std::begin(cooked_matrix))) {
            FAIL() << "Unexpected event: debounce() modified cooked matrix at " << strTime(extra_iterations, auto_advance_time)
                << "\ninput_matrix: changed=false\n" << strMatrix(input_matrix_)
                << "\nexpected_matrix:\n" << strMatrix(output_matrix_)
                << "\nactual_matrix:\n" << strMatrix(cooked_matrix);
        }

        advance_time(1);
    }
}

std::string DebounceTest::strTime(int extra_iterations, bool auto_advance_time) {
    std::stringstream text;

    text << "time " << (timer_read_fast() - time_offset_)
        << " (extra_iterations=" << extra_iterations
        << ", auto_advance_time=" << auto_advance_time << ")";

    return text.str();
}

std::string DebounceTest::strMatrix(matrix_row_t matrix[]) {
    std::stringstream text;

    text << "\t" << std::setw(3) << "";
    for (int col = 0; col < MATRIX_COLS; col++) {
        text << " " << std::setw(2) << col;
    }
    text << "\n";

    for (int row = 0; row < MATRIX_ROWS; row++) {
        text << "\t" << std::setw(2) << row << ":";
        for (int col = 0; col < MATRIX_COLS; col++) {
            text << ((matrix[row] & (1U << col)) ? " XX" : " __");
        }

        text << "\n";
    }

    return text.str();
}

DebounceTestEvent::DebounceTestEvent(fast_timer_t time,
        std::initializer_list<MatrixTestEvent> inputs,
        std::initializer_list<MatrixTestEvent> outputs)
        : time_(time), inputs_(inputs), outputs_(outputs) {
}

MatrixTestEvent::MatrixTestEvent(int row, int col, Direction direction)
        : row_(row), col_(col), direction_(direction) {
}
