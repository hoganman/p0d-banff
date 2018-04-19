package constants

import (
	"time"
)

//time constants
const (
	KSecond time.Duration = time.Second
	KMinute               = 60 * KSecond
	KHour                 = 60 * KMinute
	KDay                  = 24 * KHour

	KSleep_Confirm = 5 * KSecond
	KSleep_Submit  = 2 * KMinute
)
