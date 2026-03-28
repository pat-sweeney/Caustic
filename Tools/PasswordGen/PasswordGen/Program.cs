namespace PasswordGenerator
{
    public class PasswordGen
    {
        public static string GeneratePassword(int length)
        {
            const string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+";
            char[] password = new char[length];
            Random random = new Random((int)DateTime.UtcNow.Ticks);
            for (int i = 0; i < length; i++)
            {
                password[i] = validChars[random.Next(validChars.Length)];
            }
            return new string(password);
        }

        public static void Main(string[] args)
        {
            int passwordLength = 32; // Default length
            if (args.Length > 0 && int.TryParse(args[0], out int length))
            {
                passwordLength = length;
            }
            string password = GeneratePassword(passwordLength);
            Console.WriteLine($"Generated Password: {password}");
        }
    }
}