-- Keep a log of any SQL queries you execute as you solve the mystery.

--getting crime report
SELECT description FROM crime_scene_reports WHERE month = 7 AND year = 2021 AND day = 28;
--useful info: | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
--Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

--look at the interviews of the three witnesses :
--look at license plates from bakery footage and ruth 10 min
--atm on leggett street (withdrew money) , eugene
--less than min call for earliest flight tomorroe raymond
 SELECT transcript FROM interviews WHERE month = 7 AND year = 2021 AND day = 28;


--looking at security logs from the day of
SELECT * FROM bakery_security_logs WHERE month = 7 AND year = 2021 AND day = 28 AND hour = 10;
-- | 8      | entrance | R3G7486       |
-- | 14     | entrance | 13FNH73       |
-- | 16     | exit     | 5P2BI95       |
-- | 18     | exit     | 94KL13X       |
-- | 18     | exit     | 6P58WS2       |
-- | 19     | exit     | 4328GD8       |
-- | 20     | exit     | G412CB7       |
-- | 21     | exit     | L93JTIZ       |
-- | 23     | exit     | 322W7JE       |
-- | 23     | exit     | 0NTHK55       |
-- | 35     | exit     | 1106N58       |
-- | 42     | entrance | NRYN856       |
-- | 44     | entrance | WD5M8I6       |
-- | 55     | entrance | V47T75I
--Bruce, Lucia, Kelsey
  SELECT name, bakery_security_logs.hour, bakery_security_logs.minute FROM people
  JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
  WHERE bakery_security_logs.year = 2021
   AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28
   AND bakery_security_logs.activity = 'exit'
   AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >= 15 AND bakery_security_logs.minute <= 25
  ORDER BY bakery_security_logs.minute;

--transaction history at legett street the day of crime
SELECT account_number, amount FROM atm_transactions WHERE year = 2021 AND day = 28 AND month = 7 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
-- | 28500762       | 48     |
-- | 28296815       | 20     |
-- | 76054385       | 60     |
-- | 49610011       | 50     |
-- | 16153065       | 80     |
-- | 25506511       | 20     |
-- | 81061156       | 30     |
-- | 26013199       | 35
--finding the account number associated with name Bruce, Diana, Brooke, Kenny, Iman, Luca, Taylor, Benista
SELECT name, atm_transactions.amount FROM people
  JOIN bank_accounts ON people.id = bank_accounts.person_id
  JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
  WHERE atm_transactions.year = 2021
   AND atm_transactions.day = 28
   AND atm_transactions.month = 7
   AND atm_transactions.atm_location = 'Leggett Street'
   AND atm_transactions.transaction_type = 'withdraw';

--looking airport logs to narrow down airport and what time
SELECT origin_airport_id, destination_airport_id, hour, minute FROM flights WHERE month = 7 and day = 29 ORDER BY hour, minute;
--  origin_airport_id | destination_airport_id | hour | minute |
-- +-------------------+------------------------+------+--------+
-- | 8                 | 4                      | 8    | 20     |
-- | 8                 | 1                      | 9    | 30     |
-- | 8                 | 11                     | 12   | 15     |
-- | 8                 | 9                      | 15   | 20     |
-- | 8                 | 6                      | 16   | 0

--airport is CSF fiftyville regional airport, in fiftyville
SELECT * from airports WHERE id = 8;
-- passenger names  of the hearliest flight being 8:20 Edward, Sofia, Taylor, Bruce, Doris, Kelsey, Luca,Kenny
SELECT passengers.flight_id, name, passengers.passport_number, passengers.seat FROM people
  JOIN passengers ON people.passport_number = passengers.passport_number
  JOIN flightsON passengers.flight_id = flights.id
  WHERE flights.year = 2021
   AND flights.day = 29
   AND flights.month = 7
   AND flights.hour = 8
   AND flights.minute = 20
  ORDER BY passengers.passport_number;

  --at this point Luca and Bruce are repeats

--looking at calls that were less than a minute
SELECT caller, receiver, duration FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
--      caller     |    receiver    | duration |
-- +----------------+----------------+----------+
-- | (130) 555-0289 | (996) 555-8899 | 51       |
-- | (499) 555-9472 | (892) 555-8872 | 36       |
-- | (367) 555-5533 | (375) 555-8161 | 45       |
-- | (499) 555-9472 | (717) 555-1342 | 50       |
-- | (286) 555-6063 | (676) 555-6554 | 43       |
-- | (770) 555-1861 | (725) 555-3243 | 49       |
-- | (031) 555-6622 | (910) 555-3251 | 38       |
-- | (826) 555-1652 | (066) 555-9701 | 55       |
-- | (338) 555-6650 | (704) 555-2131 | 54       |

--in phone calls Bruce appears again, kelsey, kenny, taylor
SELECT name, phone_calls.duration FROM people
  JOIN phone_calls ON people.phone_number = phone_calls.caller
  WHERE phone_calls.year = 2021
   AND phone_calls.day = 28
   AND phone_calls.month = 7
   AND phone_calls.duration < 60
  ORDER BY phone_calls.duration;
  --same query but looking for the receiver bruce - robin
  SELECT name, phone_calls.duration FROM people
  JOIN phone_calls ON people.phone_number = phone_calls.receiver
  WHERE phone_calls.year = 2021
   AND phone_calls.day = 28
   AND phone_calls.month = 7
   AND phone_calls.duration < 60
  ORDER BY phone_calls.duration;
